import logging
import os
import tempfile

import cjyx

from DICOMLib import DICOMPlugin
from DICOMLib import DICOMLoadable
from DICOMLib import DICOMExportScene


#
# This is the plugin to handle translation of encapsulated MRML
# scenes from DICOM back into cjyx.
# It follows the DICOM module's plugin architecture.
#

class DICOMCjyxDataBundlePluginClass(DICOMPlugin):
    """ DICOM import/export plugin for Cjyx Scene Bundle
    (MRML scene file embedded in private tag of a DICOM file)
    """

    def __init__(self):
        super().__init__()
        self.loadType = "Cjyx Data Bundle"
        self.tags['seriesDescription'] = "0008,103e"
        self.tags['candygram'] = "cadb,0010"
        self.tags['zipSize'] = "cadb,1008"
        self.tags['zipData'] = "cadb,1010"

    def examineForImport(self, fileLists):
        """ Returns a list of DICOMLoadable instances
        corresponding to ways of interpreting the
        fileLists parameter.
        """
        loadables = []
        for files in fileLists:
            cachedLoadables = self.getCachedLoadables(files)
            if cachedLoadables:
                loadables += cachedLoadables
            else:
                loadablesForFiles = self.examineFiles(files)
                loadables += loadablesForFiles
                self.cacheLoadables(files, loadablesForFiles)
        return loadables

    def examineFiles(self, files):
        """ Returns a list of DICOMLoadable instances
        corresponding to ways of interpreting the
        files parameter.
        Look for the special private creator tags that indicate
        a cjyx data bundle
        Note that each data bundle is in a unique series, so
        if 'files' is a list of more than one element, then this
        is not a data bundle.
        """

        loadables = []
        if len(files) == 1:
            f = files[0]
            # get the series description to use as base for volume name
            name = cjyx.dicomDatabase.fileValue(f, self.tags['seriesDescription'])
            if name == "":
                name = "Unknown"
            candygramValue = cjyx.dicomDatabase.fileValue(f, self.tags['candygram'])

            if candygramValue:
                # default loadable includes all files for series
                loadable = DICOMLoadable()
                loadable.files = [f]
                loadable.name = name + ' - as Cjyx Scene'
                loadable.selected = True
                loadable.tooltip = 'Contains a Cjyx scene'
                loadable.confidence = 0.9
                loadables.append(loadable)
        return loadables

    def load(self, loadable):
        """Load the selection as a data bundle
        by extracting the embedded zip file and passing it to the application logic
        """

        f = loadable.files[0]

        try:
            # TODO: this method should work, but not correctly encoded in real tag
            zipSizeString = cjyx.dicomDatabase.fileValue(f, self.tags['zipSize'])
            zipSize = int(zipSizeString)
            # instead use this hack where the number is in the creator string
            candygramValue = cjyx.dicomDatabase.fileValue(f, self.tags['candygram'])
            zipSize = int(candygramValue.split(' ')[2])
        except ValueError:
            logging.error("Could not get zipSize for %s" % f)
            return False

        logging.info('importing file: %s' % f)
        logging.info('size: %d' % zipSize)

        # require that the databundle be the last element of the file
        # so we can seek from the end by the size of the zip data
        sceneDir = tempfile.mkdtemp('', 'sceneImport', cjyx.app.temporaryPath)
        fp = open(f, 'rb')

        # The previous code only works for files with odd number of bits.
        if zipSize % 2 == 0:
            fp.seek(-1 * (zipSize), os.SEEK_END)
        else:
            fp.seek(-1 * (1 + zipSize), os.SEEK_END)
        zipData = fp.read(zipSize)
        fp.close()

        # save to a temp zip file
        zipPath = os.path.join(sceneDir, 'scene.zip')
        fp = open(zipPath, 'wb')
        fp.write(zipData)
        fp.close()

        logging.info('saved zip file to: %s' % zipPath)

        nodesBeforeLoading = cjyx.util.getNodes()

        # let the scene unpack it and load it
        appLogic = cjyx.app.applicationLogic()
        sceneFile = appLogic.OpenCjyxDataBundle(zipPath, sceneDir)
        logging.info("loaded %s" % sceneFile)

        # Create subject hierarchy items for the loaded series.
        # In order for the series information are saved in the scene (and subject hierarchy
        # creation does not fail), a "main" data node needs to be selected: the first volume,
        # model, or markups node is used as series node.
        # TODO: Maybe all the nodes containing data could be added under the study, but
        #   the DICOM plugins don't support it yet.
        dataNode = None
        nodesAfterLoading = cjyx.util.getNodes()
        loadedNodes = [node for node in list(nodesAfterLoading.values()) if
                       node not in list(nodesBeforeLoading.values())]
        for node in loadedNodes:
            if node.IsA('vtkMRMLScalarVolumeNode'):
                dataNode = node
        if dataNode is None:
            for node in loadedNodes:
                if node.IsA('vtkMRMLModelNode') and node.GetName() not in ['Red Volume Slice', 'Yellow Volume Slice',
                                                                           'Green Volume Slice']:
                    dataNode = node
                    break
        if dataNode is None:
            for node in loadedNodes:
                if node.IsA('vtkMRMLMarkupsNode'):
                    dataNode = node
                    break
        if dataNode is not None:
            self.addSeriesInSubjectHierarchy(loadable, dataNode)
        else:
            logging.warning('Failed to find suitable series node in loaded scene')

        return sceneFile != ""

    def examineForExport(self, subjectHierarchyItemID):
        """Return a list of DICOMExportable instances that describe the
        available techniques that this plugin offers to convert MRML
        data into DICOM data
        """

        # Define basic properties of the exportable
        exportable = cjyx.qCjyxDICOMExportable()
        exportable.name = "Cjyx data bundle"
        exportable.tooltip = "Creates a series that embeds the entire Cjyx scene in a private DICOM tag"
        exportable.subjectHierarchyItemID = subjectHierarchyItemID
        exportable.pluginClass = self.__module__
        exportable.confidence = 0.1  # There could be more specialized volume types

        # Do not define tags (exportable.setTag) because they would overwrite values in the reference series

        return [exportable]

    def export(self, exportables):
        shNode = cjyx.vtkMRMLSubjectHierarchyNode.GetSubjectHierarchyNode(cjyx.mrmlScene)
        if shNode is None:
            error = "Invalid subject hierarchy"
            logging.error(error)
            return error
        dicomFiles = []
        for exportable in exportables:
            # Find reference series (series that will be modified into a scene data bundle)
            # Get DICOM UID - can be study instance UID or series instance UID
            dicomUid = shNode.GetItemUID(exportable.subjectHierarchyItemID,
                                         cjyx.vtkMRMLSubjectHierarchyConstants.GetDICOMUIDName())
            if not dicomUid:
                continue
            # Get series instance UID
            if shNode.GetItemLevel(exportable.subjectHierarchyItemID) == cjyx.vtkMRMLSubjectHierarchyConstants.GetDICOMLevelStudy():
                # Study is selected
                seriesInstanceUids = cjyx.dicomDatabase.seriesForStudy(dicomUid)
                seriesInstanceUid = seriesInstanceUids[0] if seriesInstanceUids else None
            else:
                # Series is selected
                seriesInstanceUid = dicomUid
            # Get first file of the series
            dicomFiles = cjyx.dicomDatabase.filesForSeries(seriesInstanceUid, 1)
            if not dicomFiles:
                continue
            break
        if not dicomFiles:
            error = "Cjyx data bundle export failed. No file is found for any of the selected items."
            logging.error(error)
            return error

        # Assemble tags dictionary for volume export
        tags = {}
        tags['PatientName'] = exportable.tag(cjyx.vtkMRMLSubjectHierarchyConstants.GetDICOMPatientNameTagName())
        tags['PatientID'] = exportable.tag(cjyx.vtkMRMLSubjectHierarchyConstants.GetDICOMPatientIDTagName())
        tags['PatientBirthDate'] = exportable.tag(
            cjyx.vtkMRMLSubjectHierarchyConstants.GetDICOMPatientBirthDateTagName())
        tags['PatientSex'] = exportable.tag(cjyx.vtkMRMLSubjectHierarchyConstants.GetDICOMPatientSexTagName())
        tags['PatientComments'] = exportable.tag(
            cjyx.vtkMRMLSubjectHierarchyConstants.GetDICOMPatientCommentsTagName())

        tags['StudyDate'] = exportable.tag(cjyx.vtkMRMLSubjectHierarchyConstants.GetDICOMStudyDateTagName())
        tags['StudyTime'] = exportable.tag(cjyx.vtkMRMLSubjectHierarchyConstants.GetDICOMStudyTimeTagName())
        tags['StudyDescription'] = exportable.tag(
            cjyx.vtkMRMLSubjectHierarchyConstants.GetDICOMStudyDescriptionTagName())

        # Perform export
        exporter = DICOMExportScene(dicomFiles[0], exportable.directory)
        exporter.optionalTags = tags
        exporter.export()

        # Success
        return ""


#
# DICOMCjyxDataBundlePlugin
#

class DICOMCjyxDataBundlePlugin:
    """
    This class is the 'hook' for cjyx to detect and recognize the plugin
    as a loadable scripted module
    """

    def __init__(self, parent):
        parent.title = "DICOM Diffusion Volume Plugin"
        parent.categories = ["Developer Tools.DICOM Plugins"]
        parent.contributors = ["Steve Pieper (Isomics Inc.), Csaba Pinter (Pixel Medical, Inc.)"]
        parent.helpText = """
Plugin to the DICOM Module to parse and load diffusion volumes
from DICOM files.
No module interface here, only in the DICOM module
"""
        parent.acknowledgementText = """
This DICOM Plugin was developed by
Steve Pieper, Isomics, Inc.
and was partially funded by NIH grant 3P41RR013218.
"""

        # don't show this module - it only appears in the DICOM module
        parent.hidden = True

        # Add this extension to the DICOM module's list for discovery when the module
        # is created.  Since this module may be discovered before DICOM itself,
        # create the list if it doesn't already exist.
        try:
            cjyx.modules.dicomPlugins
        except AttributeError:
            cjyx.modules.dicomPlugins = {}
        cjyx.modules.dicomPlugins['DICOMCjyxDataBundlePlugin'] = DICOMCjyxDataBundlePluginClass


#
# DICOMCjyxDataBundleWidget
#

class DICOMCjyxDataBundleWidget:
    def __init__(self, parent=None):
        self.parent = parent

    def setup(self):
        # don't display anything for this widget - it will be hidden anyway
        pass

    def enter(self):
        pass

    def exit(self):
        pass

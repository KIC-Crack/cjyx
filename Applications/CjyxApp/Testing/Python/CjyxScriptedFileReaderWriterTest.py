import logging
import os

import cjyx
from cjyx.ScriptedLoadableModule import *


class CjyxScriptedFileReaderWriterTest(ScriptedLoadableModule):
    def __init__(self, parent):
        ScriptedLoadableModule.__init__(self, parent)
        parent.title = 'CjyxScriptedFileReaderWriterTest'
        parent.categories = ['Testing.TestCases']
        parent.dependencies = []
        parent.contributors = ["Andras Lasso (PerkLab, Queen's)"]
        parent.helpText = '''
    This module is used to test qCjyxScriptedFileReader and qCjyxScriptedFileWriter classes.
    '''
        parent.acknowledgementText = '''
    This file was originally developed by Andras Lasso, PerkLab.
    '''
        self.parent = parent


class CjyxScriptedFileReaderWriterTestWidget(ScriptedLoadableModuleWidget):
    def setup(self):
        ScriptedLoadableModuleWidget.setup(self)
        # Default reload&test widgets are enough.
        # Note that reader and writer is not reloaded.


class CjyxScriptedFileReaderWriterTestFileReader:

    def __init__(self, parent):
        self.parent = parent

    def description(self):
        return 'My file type'

    def fileType(self):
        return 'MyFileType'

    def extensions(self):
        return ['My file type (*.mft)']

    def canLoadFile(self, filePath):
        # Only enable this reader in testing mode
        if not cjyx.app.testingEnabled():
            return False

        firstLine = ''
        with open(filePath) as f:
            firstLine = f.readline()
        validFile = 'magic' in firstLine
        return validFile

    def load(self, properties):
        try:
            filePath = properties['fileName']

            # Get node base name from filename
            if 'name' in properties.keys():
                baseName = properties['name']
            else:
                baseName = os.path.splitext(os.path.basename(filePath))[0]
                baseName = cjyx.mrmlScene.GenerateUniqueName(baseName)

            # Read file content
            with open(filePath) as myfile:
                data = myfile.readlines()

            # Check if file is valid
            firstLine = data[0].rstrip()
            if firstLine != 'magic':
                raise ValueError('Cannot read file, it is expected to start with magic')

            # Load content into new node
            loadedNode = cjyx.mrmlScene.AddNewNodeByClass('vtkMRMLTextNode', baseName)
            loadedNode.SetText(''.join(data[1:]))

            # Uncomment the next line to display a warning message to the user.
            # self.parent.userMessages().AddMessage(vtk.vtkCommand.WarningEvent, "This is a warning message")

        except Exception as e:
            logging.error('Failed to load file: ' + str(e))
            import traceback
            traceback.print_exc()
            return False

        self.parent.loadedNodes = [loadedNode.GetID()]
        return True


class CjyxScriptedFileReaderWriterTestFileWriter:

    def __init__(self, parent):
        self.parent = parent

    def description(self):
        return 'My file type'

    def fileType(self):
        return 'MyFileType'

    def extensions(self, obj):
        return ['My file type (*.mft)']

    def canWriteObject(self, obj):
        # Only enable this writer in testing mode
        if not cjyx.app.testingEnabled():
            return False

        return bool(obj.IsA("vtkMRMLTextNode"))

    def write(self, properties):
        try:

            # Get node
            node = cjyx.mrmlScene.GetNodeByID(properties["nodeID"])

            # Write node content to file
            filePath = properties['fileName']
            with open(filePath, 'w') as myfile:
                myfile.write("magic\n")
                myfile.write(node.GetText())

        except Exception as e:
            logging.error('Failed to write file: ' + str(e))
            import traceback
            traceback.print_exc()
            return False

        self.parent.writtenNodes = [node.GetID()]
        return True


class CjyxScriptedFileReaderWriterTestTest(ScriptedLoadableModuleTest):
    def runTest(self):
        """Run as few or as many tests as needed here.
        """
        self.setUp()
        self.test_Writer()
        self.test_Reader()
        self.tearDown()
        self.delayDisplay('Testing complete')

    def setUp(self):
        self.tempDir = cjyx.util.tempDirectory()
        logging.info("tempDir: " + self.tempDir)
        self.textInNode = "This is\nsome example test"
        self.validFilename = self.tempDir + "/tempCjyxScriptedFileReaderWriterTestValid.mft"
        self.invalidFilename = self.tempDir + "/tempCjyxScriptedFileReaderWriterTestInvalid.mft"
        cjyx.mrmlScene.Clear()

    def tearDown(self):
        import shutil
        shutil.rmtree(self.tempDir, True)

    def test_WriterReader(self):
        # Writer and reader tests are put in the same function to ensure
        # that writing is done before reading (it generates input data for reading).

        self.delayDisplay('Testing node writer')
        cjyx.mrmlScene.Clear()
        textNode = cjyx.mrmlScene.AddNewNodeByClass('vtkMRMLTextNode')
        textNode.SetText(self.textInNode)
        self.assertTrue(cjyx.util.saveNode(textNode, self.validFilename, {'fileType': 'MyFileType'}))

        self.delayDisplay('Testing node reader')
        cjyx.mrmlScene.Clear()
        loadedNode = cjyx.util.loadNodeFromFile(self.validFilename, 'MyFileType')
        self.assertIsNotNone(loadedNode)
        self.assertTrue(loadedNode.IsA('vtkMRMLTextNode'))
        self.assertEqual(loadedNode.GetText(), self.textInNode)

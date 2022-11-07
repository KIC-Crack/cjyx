import vtk

import cjyx
from cjyx.ScriptedLoadableModule import *


#
# CjyxMRBMultipleSaveRestoreTest
#

class CjyxMRBMultipleSaveRestoreTest(ScriptedLoadableModule):
    """Uses ScriptedLoadableModule base class, available at:
    https://github.com/Slicer/Slicer/blob/master/Base/Python/slicer/ScriptedLoadableModule.py
    """

    def __init__(self, parent):
        ScriptedLoadableModule.__init__(self, parent)
        parent.title = "CjyxMRBMultipleSaveRestoreTest"
        parent.categories = ["Testing.TestCases"]
        parent.contributors = ["Nicole Aucoin (BWH)"]
        parent.helpText = """
    Self test for MRB and Scene Views multiple save.
    No module interface here, only used in SelfTests module
    """
        parent.acknowledgementText = """
    This tes was developed by
    Nicole Aucoin, BWH
    and was partially funded by NIH grant 3P41RR013218.
    """


#
# CjyxMRBMultipleSaveRestoreTestWidget
#

class CjyxMRBMultipleSaveRestoreTestWidget(ScriptedLoadableModuleWidget):
    """Uses ScriptedLoadableModuleWidget base class, available at:
    https://github.com/Slicer/Slicer/blob/master/Base/Python/slicer/ScriptedLoadableModule.py
    """

    def setup(self):
        ScriptedLoadableModuleWidget.setup(self)


class CjyxMRBMultipleSaveRestore(ScriptedLoadableModuleTest):
    """
    This is the test case for your scripted module.
    Uses ScriptedLoadableModuleTest base class, available at:
    https://github.com/Slicer/Slicer/blob/master/Base/Python/slicer/ScriptedLoadableModule.py
    """

    def __init__(self, methodName='runTest', uniqueDirectory=True, strict=False):
        """
        Tests the use of mrml and mrb save formats with volumes and markups points lists.
        Checks that scene views are saved and restored as expected.
        Checks that after a scene view restore, MRB save and reload works as expected.

        uniqueDirectory: boolean about save directory
                         False to reuse standard dir name
                         True timestamps dir name
        strict: boolean about how carefully to check result
                         True then check every detail
                         False then confirm basic operation, but allow non-critical issues to pass
        """
        ScriptedLoadableModuleTest.__init__(self, methodName)
        self.uniqueDirectory = uniqueDirectory
        self.strict = strict

    def setUp(self):
        cjyx.mrmlScene.Clear(0)

    def runTest(self):
        self.setUp()
        self.test_CjyxMRBMultipleSaveRestore()

    def test_CjyxMRBMultipleSaveRestore(self):
        """
        Replicate the issue reported in bug 3956 where saving
        and restoring an MRB file does not work.
        """

        print("Running CjyxMRBMultipleSaveRestore Test case with:")
        print("uniqueDirectory : %s" % self.uniqueDirectory)
        print("strict : %s" % self.strict)

        #
        # first, get the data
        #
        print("Getting MR Head Volume")
        import SampleData
        mrHeadVolume = SampleData.downloadSample("MRHead")

        # Place a control point
        pointListNode = cjyx.mrmlScene.AddNewNodeByClass("vtkMRMLMarkupsFiducialNode", "F")
        pointListNode.CreateDefaultDisplayNodes()
        eye = [33.4975, 79.4042, -10.2143]
        nose = [-2.145, 116.14, -43.31]
        fidIndexEye = pointListNode.AddControlPoint(eye)
        fidIndexNose = pointListNode.AddControlPoint(nose)

        self.delayDisplay('Finished with download and placing markups points\n')

        # confirm that MRHead is in the background of the Red slice
        redComposite = cjyx.util.getNode('vtkMRMLSliceCompositeNodeRed')
        mrHead = cjyx.util.getNode('MRHead')
        self.assertEqual(redComposite.GetBackgroundVolumeID(), mrHead.GetID())
        self.delayDisplay('The MRHead volume is in the background of the Red viewer')

        # turn off visibility save scene view
        pointListNode.SetDisplayVisibility(0)
        self.delayDisplay('Not showing markup points')
        self.storeSceneView('Invisible-view', "Not showing markup points")
        pointListNode.SetDisplayVisibility(1)
        self.delayDisplay('Showing markup points')
        self.storeSceneView('Visible-view', "Showing markup points")

        #
        # save the mrml scene to a temp directory, then zip it
        #
        applicationLogic = cjyx.app.applicationLogic()
        sceneSaveDirectory = cjyx.util.tempDirectory('__scene__')
        mrbFilePath = cjyx.util.tempDirectory('__mrb__') + '/CjyxMRBMultipleSaveRestore-1.mrb'
        self.delayDisplay("Saving scene to: %s\n" % sceneSaveDirectory + "Saving mrb to: %s" % mrbFilePath)
        self.assertTrue(
            applicationLogic.SaveSceneToCjyxDataBundleDirectory(sceneSaveDirectory, None)
        )
        self.delayDisplay("Finished saving scene")
        self.assertTrue(
            applicationLogic.Zip(mrbFilePath, sceneSaveDirectory)
        )
        self.delayDisplay("Finished saving MRB")
        self.delayDisplay("Cjyx mrml scene root dir after first save = %s" % cjyx.mrmlScene.GetRootDirectory())

        #
        # reload the mrb and restore a scene view
        #
        cjyx.mrmlScene.Clear(0)
        mrbExtractPath = cjyx.util.tempDirectory('__mrb_extract__')
        self.delayDisplay('Now, reload the saved MRB')
        mrbLoaded = applicationLogic.OpenCjyxDataBundle(mrbFilePath, mrbExtractPath)
        # load can return false even though it succeeded - only fail if in strict mode
        self.assertTrue(not self.strict or mrbLoaded)
        cjyx.app.processEvents()

        # confirm again that MRHead is in the background of the Red slice
        self.delayDisplay('Is the MHRead volume AGAIN in the background of the Red viewer?')
        redComposite = cjyx.util.getNode('vtkMRMLSliceCompositeNodeRed')
        mrHead = cjyx.util.getNode('MRHead')
        self.assertEqual(redComposite.GetBackgroundVolumeID(), mrHead.GetID())
        self.delayDisplay('The MRHead volume is AGAIN in the background of the Red viewer')

        # confirm that the point list exists with two points
        self.delayDisplay('Does the point list have 2 points in it?')
        pointListNode = cjyx.util.getNode('F')
        self.assertEqual(pointListNode.GetNumberOfControlPoints(), 2)
        self.delayDisplay('The point list has 2 points in it')

        # Restore the invisible scene view
        self.delayDisplay('About to restore Invisible-view scene')
        sceneView = cjyx.util.getNode('Invisible-view')
        sceneView.RestoreScene()
        pointListNode = cjyx.util.getNode('F')
        self.assertEqual(pointListNode.GetDisplayVisibility(), 0)
        self.delayDisplay("NOT seeing the points")
        self.delayDisplay('Does the point list still have 2 points in it after restoring a scenen view?')
        self.assertEqual(pointListNode.GetNumberOfControlPoints(), 2)
        self.delayDisplay('The point list has 2 points in it after scene view restore')

        #
        # Save it again
        #
        sceneSaveDirectory = cjyx.util.tempDirectory('__scene2__')
        mrbFilePath = cjyx.util.tempDirectory('__mrb__') + '/CjyxMRBMultipleSaveRestore-2.mrb'
        self.delayDisplay("Saving scene to: %s\n" % sceneSaveDirectory + "Saving mrb to: %s" % mrbFilePath)
        self.assertTrue(
            applicationLogic.SaveSceneToCjyxDataBundleDirectory(sceneSaveDirectory, None)
        )
        self.delayDisplay("Finished saving scene after restoring a scene view")
        self.assertTrue(
            applicationLogic.Zip(mrbFilePath, sceneSaveDirectory)
        )
        self.delayDisplay("Finished saving MRB after restoring a scene view")

        self.delayDisplay("Cjyx mrml scene root dir after second save = %s" % cjyx.mrmlScene.GetRootDirectory())

        #
        # reload the second mrb and test
        #
        cjyx.mrmlScene.Clear(0)
        mrbExtractPath = cjyx.util.tempDirectory('__mrb_extract2__')
        self.delayDisplay('Now, reload the second saved MRB %s' % mrbFilePath)
        mrbLoaded = applicationLogic.OpenCjyxDataBundle(mrbFilePath, mrbExtractPath)
        # load can return false even though it succeeded - only fail if in strict mode
        self.assertTrue(not self.strict or mrbLoaded)
        cjyx.app.processEvents()

        # confirm that MRHead is in the background of the Red slice after mrb reload
        self.delayDisplay('MRHead volume is the background of the Red viewer after mrb reload?')
        redComposite = cjyx.util.getNode('vtkMRMLSliceCompositeNodeRed')
        fa = cjyx.util.getNode('MRHead')
        self.assertEqual(redComposite.GetBackgroundVolumeID(), mrHead.GetID())
        self.delayDisplay('Yes, the MRHead volume is back in the background of the Red viewer')

        # confirm that the point list exists with two points
        pointListNode = cjyx.util.getNode('F')
        self.assertEqual(pointListNode.GetNumberOfControlPoints(), 2)
        self.delayDisplay('The point list has 2 points in it after scene view restore, save and MRB reload')
        self.assertEqual(pointListNode.GetDisplayVisibility(), 0)
        self.delayDisplay("NOT seeing the points")

        self.delayDisplay("Test Finished")

    def storeSceneView(self, name, description=""):
        """  Store a scene view into the current scene.
        TODO: this might move to cjyx.util
        """
        layoutManager = cjyx.app.layoutManager()

        sceneViewNode = cjyx.vtkMRMLSceneViewNode()
        view1 = layoutManager.threeDWidget(0).threeDView()

        w2i1 = vtk.vtkWindowToImageFilter()
        w2i1.SetInput(view1.renderWindow())

        w2i1.Update()
        image1 = w2i1.GetOutput()
        sceneViewNode.SetScreenShot(image1)
        sceneViewNode.UpdateStoredScene()
        cjyx.mrmlScene.AddNode(sceneViewNode)

        sceneViewNode.SetName(name)
        sceneViewNode.SetSceneViewDescription(description)
        sceneViewNode.StoreScene()

        return sceneViewNode

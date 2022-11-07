import cjyx
from cjyx.ScriptedLoadableModule import *

#
# CropVolumeSelfTest
#


class CropVolumeSelfTest(ScriptedLoadableModule):
    def __init__(self, parent):
        ScriptedLoadableModule.__init__(self, parent)
        parent.title = "CropVolumeSelfTest"  # TODO make this more human readable by adding spaces
        parent.categories = ["Testing.TestCases"]
        parent.dependencies = []
        parent.contributors = ["Andrey Fedorov (BWH)"]  # replace with "Firstname Lastname (Org)"
        parent.helpText = """
    This module was developed as a self test to perform the operations needed for crop volume.
    """
        parent.acknowledgementText = """
"""  # replace with organization, grant and thanks.


#
# qCropVolumeSelfTestWidget
#

class CropVolumeSelfTestWidget(ScriptedLoadableModuleWidget):
    """Uses ScriptedLoadableModuleWidget base class, available at:
    https://github.com/Slicer/Slicer/blob/master/Base/Python/slicer/ScriptedLoadableModule.py
    """

    def setup(self):
        ScriptedLoadableModuleWidget.setup(self)
        # Instantiate and connect widgets ...

        # Add vertical spacer
        self.layout.addStretch(1)


class CropVolumeSelfTestTest(ScriptedLoadableModuleTest):
    """
    This is the test case for your scripted module.
    """

    def setUp(self):
        cjyx.mrmlScene.Clear(0)

    def runTest(self):
        self.setUp()
        self.test_CropVolumeSelfTest()

    def test_CropVolumeSelfTest(self):
        """
        Replicate the crashe in issue 3117
        """

        print("Running CropVolumeSelfTest Test case:")

        import SampleData

        vol = SampleData.downloadSample("MRHead")
        roi = cjyx.vtkMRMLMarkupsROINode()

        mainWindow = cjyx.util.mainWindow()
        mainWindow.moduleSelector().selectModule('CropVolume')

        cropVolumeNode = cjyx.vtkMRMLCropVolumeParametersNode()
        cropVolumeNode.SetScene(cjyx.mrmlScene)
        cropVolumeNode.SetName('ChangeTracker_CropVolume_node')
        cropVolumeNode.SetIsotropicResampling(True)
        cropVolumeNode.SetSpacingScalingConst(0.5)
        cjyx.mrmlScene.AddNode(cropVolumeNode)

        cropVolumeNode.SetInputVolumeNodeID(vol.GetID())
        cropVolumeNode.SetROINodeID(roi.GetID())

        cropVolumeLogic = cjyx.modules.cropvolume.logic()
        cropVolumeLogic.Apply(cropVolumeNode)

        self.delayDisplay('First test passed, closing the scene and running again')
        # test clearing the scene and running a second time
        cjyx.mrmlScene.Clear(0)
        # the module will re-add the removed parameters node
        mainWindow.moduleSelector().selectModule('Transforms')
        mainWindow.moduleSelector().selectModule('CropVolume')
        cropVolumeNode = cjyx.mrmlScene.GetNodeByID('vtkMRMLCropVolumeParametersNode1')
        vol = SampleData.downloadSample("MRHead")
        roi = cjyx.vtkMRMLMarkupsROINode()
        cropVolumeNode.SetInputVolumeNodeID(vol.GetID())
        cropVolumeNode.SetROINodeID(roi.GetID())
        cropVolumeLogic.Apply(cropVolumeNode)

        self.delayDisplay('Test passed')

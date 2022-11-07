import cjyx
from cjyx.ScriptedLoadableModule import ScriptedLoadableModuleTest


class VolumeRenderingThreeDOnlyLayout(ScriptedLoadableModuleTest):
    """
    This is the test case for your scripted module.
    Uses ScriptedLoadableModuleTest base class, available at:
    https://github.com/Slicer/Slicer/blob/master/Base/Python/slicer/ScriptedLoadableModule.py
    """

    def setUp(self):
        pass

    def tearDown(self):
        cjyx.mrmlScene.Clear(0)

    def test_VolumeRenderThreeDOnlyLayout(self):
        """
        Test that the following workflow does not segfault:
        - Set 3D-only layout, reinitialize slice widgets
        - Load volume
        - Enter the volume rendering module
        """
        self.delayDisplay("Starting the test")

        # Set 3D-only layout
        layoutManager = cjyx.app.layoutManager()
        layoutManager.setLayout(cjyx.vtkMRMLLayoutNode.CjyxLayoutOneUp3DView)

        # Reinitialize MRML scene to force re-creating slice widgets
        mrmlScene = layoutManager.mrmlScene()
        layoutManager.setMRMLScene(None)
        layoutManager.setMRMLScene(mrmlScene)

        # Load MRHead volume
        import SampleData
        SampleData.downloadSample("MRHead")

        # Enter the volume rendering module
        cjyx.util.mainWindow().moduleSelector().selectModule('VolumeRendering')

        self.delayDisplay('Test passed!')

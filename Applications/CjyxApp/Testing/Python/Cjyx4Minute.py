import ctk
import qt

import cjyx
from cjyx.ScriptedLoadableModule import *
from cjyx.util import TESTING_DATA_URL


#
# Cjyx4Minute
#

class Cjyx4Minute(ScriptedLoadableModule):
    """Uses ScriptedLoadableModule base class, available at:
    https://github.com/Slicer/Slicer/blob/master/Base/Python/slicer/ScriptedLoadableModule.py
    """

    def __init__(self, parent):
        ScriptedLoadableModule.__init__(self, parent)
        parent.title = "Cjyx4Minute"  # TODO make this more human readable by adding spaces
        parent.categories = ["Testing.TestCases"]
        parent.dependencies = []
        parent.contributors = ["Jim Miller (GE)"]  # replace with "Firstname Lastname (Org)"
        parent.helpText = """
    Test suite for the Cjyx 4 Minute tutorial
    """
        parent.acknowledgementText = """
    This file was originally developed by Jim Miller, GE and was partially funded by NIH grant U54EB005149.
"""  # replace with organization, grant and thanks.


#
# qCjyx4MinuteWidget
#

class Cjyx4MinuteWidget(ScriptedLoadableModuleWidget):
    """Uses ScriptedLoadableModuleWidget base class, available at:
    https://github.com/Slicer/Slicer/blob/master/Base/Python/slicer/ScriptedLoadableModule.py
    """

    def setup(self):
        ScriptedLoadableModuleWidget.setup(self)
        # Instantiate and connect widgets ...

        # Collapsible button
        dummyCollapsibleButton = ctk.ctkCollapsibleButton()
        dummyCollapsibleButton.text = "A collapsible button"
        self.layout.addWidget(dummyCollapsibleButton)

        # Layout within the dummy collapsible button
        dummyFormLayout = qt.QFormLayout(dummyCollapsibleButton)

        # HelloWorld button
        helloWorldButton = qt.QPushButton("Hello world")
        helloWorldButton.toolTip = "Print 'Hello world' in standard output."
        dummyFormLayout.addWidget(helloWorldButton)
        helloWorldButton.connect('clicked(bool)', self.onHelloWorldButtonClicked)

        # Add vertical spacer
        self.layout.addStretch(1)

        # Set local var as instance attribute
        self.helloWorldButton = helloWorldButton

    def onHelloWorldButtonClicked(self):
        print("Hello World !")


#
# Cjyx4MinuteLogic
#

class Cjyx4MinuteLogic(ScriptedLoadableModuleLogic):
    """This class should implement all the actual
    computation done by your module.  The interface
    should be such that other python code can import
    this class and make use of the functionality without
    requiring an instance of the Widget.
    Uses ScriptedLoadableModuleLogic base class, available at:
    https://github.com/Slicer/Slicer/blob/master/Base/Python/slicer/ScriptedLoadableModule.py
    """

    def hasImageData(self, volumeNode):
        """This is a dummy logic method that
        returns true if the passed in volume
        node has valid image data
        """
        if not volumeNode:
            print('no volume node')
            return False
        if volumeNode.GetImageData() is None:
            print('no image data')
            return False
        return True


class Cjyx4MinuteTest(ScriptedLoadableModuleTest):
    """
    This is the test case for your scripted module.
    Uses ScriptedLoadableModuleTest base class, available at:
    https://github.com/Slicer/Slicer/blob/master/Base/Python/slicer/ScriptedLoadableModule.py
    """

    def setUp(self):
        """ Do whatever is needed to reset the state - typically a scene clear will be enough.
        """
        cjyx.mrmlScene.Clear(0)

    def runTest(self):
        """Run as few or as many tests as needed here.
        """
        self.setUp()
        self.test_Cjyx4Minute1()

    def test_Cjyx4Minute1(self):
        """ Tests parts of the Cjyx4Minute tutorial.

        Currently testing 'Part 2' which covers volumes, models, visibility and clipping.
        """
        self.delayDisplay("Starting the test")

        logic = Cjyx4MinuteLogic()

        #
        # first, get some data
        #
        import SampleData
        SampleData.downloadFromURL(
            fileNames='cjyx4minute.mrb',
            loadFiles=True,
            uris=TESTING_DATA_URL + 'SHA256/5a1c78c3347f77970b1a29e718bfa10e5376214692d55a7320af94b9d8d592b8',
            checksums='SHA256:5a1c78c3347f77970b1a29e718bfa10e5376214692d55a7320af94b9d8d592b8')
        self.delayDisplay('Finished with download and loading')

        # Testing "Part 2" of Tutorial
        #
        #
        self.delayDisplay('Testing Part 2 of the Tutorial')

        # check volume is loaded out of scene
        volumeNode = cjyx.util.getNode(pattern="grayscale")
        self.assertIsNotNone(logic.hasImageData(volumeNode))

        # check the slice planes
        red = cjyx.util.getNode(pattern="vtkMRMLSliceNode1")
        red.SetSliceVisible(1)

        green = cjyx.util.getNode(pattern="vtkMRMLSliceNode3")
        green.SetSliceVisible(1)

        # rotate a bit
        cam = cjyx.util.getNode(pattern='vtkMRMLCameraNode1')
        cam.GetCamera().Azimuth(90)
        cam.GetCamera().Elevation(20)

        # turn off skin and skull
        skin = cjyx.util.getNode(pattern='Skin')
        skin.GetDisplayNode().SetVisibility(0)

        skull = cjyx.util.getNode(pattern='skull_bone')
        skull.GetDisplayNode().SetVisibility(0)

        # clip the model hemispheric_white_matter.vtk
        m = cjyx.util.mainWindow()
        m.moduleSelector().selectModule('Models')

        models = cjyx.util.getModule('Models')
        logic = models.logic()

        hemispheric_white_matter = cjyx.util.getNode(pattern='hemispheric_white_matter')
        hemispheric_white_matter.GetDisplayNode().SetClipping(1)

        clip = cjyx.util.getNode('ClipModelsParameters1')
        clip.SetRedSliceClipState(0)
        clip.SetYellowSliceClipState(0)
        clip.SetGreenSliceClipState(2)

        # Can we make this more than just a Smoke Test?
        self.delayDisplay('Optic chiasm should be visible. Front part of white matter should be clipped.')

        # Done
        #
        #
        self.delayDisplay('Test passed!')

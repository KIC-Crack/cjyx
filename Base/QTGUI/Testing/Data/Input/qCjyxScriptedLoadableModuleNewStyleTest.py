class qCjyxScriptedLoadableModuleNewStyleTest:
    def __init__(self, parent):
        parent.title = "qCjyxScriptedLoadableModuleNewStyle Test"
        parent.categories = ["Testing"]
        parent.associatedNodeTypes = ["vtkMRMLModelNode", "vtkMRMLScalarVolumeNode"]
        parent.contributors = ["Jean-Christophe Fillion-Robin (Kitware); Max Smolens (Kitware)"]
        parent.helpText = """
    This module is for testing.
    """
        parent.acknowledgementText = """
    Based on qCjyxScriptedLoadableModuleTest .
    """
        self.parent = parent

    def setup(self):
        self.parent.setProperty('setup_called_within_Python', True)

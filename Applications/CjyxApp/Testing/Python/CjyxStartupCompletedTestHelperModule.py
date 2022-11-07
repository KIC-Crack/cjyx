import cjyx
import os

from cjyx.ScriptedLoadableModule import *


class CjyxStartupCompletedTestHelperModule(ScriptedLoadableModule):

    def __init__(self, parent):
        ScriptedLoadableModule.__init__(self, parent)
        self.parent.title = "CjyxStartupCompletedTest"
        self.parent.categories = ["Testing.TestCases"]
        self.parent.contributors = ["Jean-Christophe Fillion-Robin (Kitware), Andras Lasso (PerkLab)"]
        self.parent.widgetRepresentationCreationEnabled = False

        self.testOutputFileName = os.environ['CJYX_STARTUP_COMPLETED_TEST_OUTPUT']
        if os.path.isfile(self.testOutputFileName):
            os.remove(self.testOutputFileName)

        cjyx.app.connect("startupCompleted()", self.onStartupCompleted)

        print("CjyxStartupCompletedTestHelperModule initialized")

    def onStartupCompleted(self):
        print("StartupCompleted emitted")
        import os
        fd = os.open(self.testOutputFileName, os.O_RDWR | os.O_CREAT)
        os.write(fd, 'CjyxStartupCompletedTestHelperModule.py generated this file')
        os.write(fd, 'when cjyx.app emitted startupCompleted() signal\n')
        os.close(fd)

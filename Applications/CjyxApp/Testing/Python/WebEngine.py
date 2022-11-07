import ctk
import qt

import cjyx
from cjyx.ScriptedLoadableModule import *


#
# WebEngine
#

class WebEngine(ScriptedLoadableModule):
    """Uses ScriptedLoadableModule base class, available at:
    https://github.com/Slicer/Slicer/blob/master/Base/Python/slicer/ScriptedLoadableModule.py
    """

    def __init__(self, parent):
        ScriptedLoadableModule.__init__(self, parent)
        parent.title = "WebEngine"
        parent.categories = ["Testing.TestCases"]
        parent.dependencies = []
        parent.contributors = ["Steve Pieper (Isomics)"]
        parent.helpText = """
    Module to test WebEngine.
    """
        parent.acknowledgementText = """
    This file was originally developed by Steve Pieper and was partially funded by NSF grant 1759883
"""  # replace with organization, grant and thanks.


#
# qWebEngineWidget
#

class WebEngineWidget(ScriptedLoadableModuleWidget):
    """Uses ScriptedLoadableModuleWidget base class, available at:
    https://github.com/Slicer/Slicer/blob/master/Base/Python/slicer/ScriptedLoadableModule.py
    """

    def __init__(self, parent):
        ScriptedLoadableModuleWidget.__init__(self, parent)
        self.webWidgets = []  # hold references so windows persist

    def setup(self):
        ScriptedLoadableModuleWidget.setup(self)
        # Instantiate and connect widgets ...

        # Collapsible button
        sitesCollapsibleButton = ctk.ctkCollapsibleButton()
        sitesCollapsibleButton.text = "Sample Sites"
        self.layout.addWidget(sitesCollapsibleButton)

        # Layout within the collapsible button
        sitesFormLayout = qt.QFormLayout(sitesCollapsibleButton)

        # site buttons
        buttons = []
        self.sites = [
            {
                "label": "Web Console", "url": "http://localhost:1337"
            },
            {
                "label": "Crowds Cure Cancer", "url": "http://cancer.crowds-cure.org"
            },
            {
                "label": "Cjyx Home Page", "url": "https://slicer.org"
            },
            {
                "label": "MorphoSource", "url": "https://www.morphosource.org"
            },
            {
                "label": "Cjyx SampleData", "url": "https://www.slicer.org/wiki/SampleData"
            },
            {
                "label": "CjyxMorph", "url": "https://slicermorph.github.io"
            },
        ]
        for site in self.sites:
            button = qt.QPushButton(site["label"])
            button.toolTip = "Open %s" % site["url"]
            sitesFormLayout.addWidget(button)
            onClick = lambda click, site=site: self.onSiteButtonClicked(site)
            button.connect('clicked(bool)', onClick)
            buttons.append(button)

        button = qt.QPushButton("Close All")
        button.toolTip = "Close all the web views"
        button.connect('clicked(bool)', self.onCloseAll)
        self.layout.addWidget(button)

        # Add vertical spacer
        self.layout.addStretch(1)

    def onSiteButtonClicked(self, site):
        webWidget = cjyx.qCjyxWebWidget()
        cjyxGeometry = cjyx.util.mainWindow().geometry
        webWidget.size = qt.QSize(1536, 1024)
        webWidget.pos = qt.QPoint(cjyxGeometry.x() + 256, cjyxGeometry.y() + 128)
        webWidget.url = site["url"]
        webWidget.show()
        self.webWidgets.append(webWidget)

    def onCloseAll(self):
        for widget in self.webWidgets:
            del widget
        self.webWidgets = []


class WebEngineTest(ScriptedLoadableModuleTest):
    """
    This is the test case for your scripted module.
    Uses ScriptedLoadableModuleTest base class, available at:
    https://github.com/Slicer/Slicer/blob/master/Base/Python/slicer/ScriptedLoadableModule.py
    """

    def setUp(self):
        """ Do whatever is needed to reset the state - typically a scene clear will be enough.
        """
        self.gotResponse = False
        self.gotCorrectResponse = False

    def runTest(self):
        """Run as few or as many tests as needed here.
        """
        self.setUp()
        self.test_WebEngine1()

    def onEvalResult(self, js, result):
        if js == "valueFromCjyx;":
            self.delayDisplay("Got Cjyx result back from JavaScript")
            self.gotResponse = True
            if result == "42":
                self.gotCorrectResponse = True
                self.delayDisplay("Got the expected result back from JavaScript")
        else:
            self.delayDisplay("Got a result back from JavaScript")
            print((js, result))

    def test_WebEngine1(self):
        """ Testing WebEngine
        """

        self.delayDisplay("Starting the test")

        webWidget = cjyx.qCjyxWebWidget()
        webWidget.size = qt.QSize(1024, 512)
        webWidget.webView().url = qt.QUrl("")
        webWidget.show()
        self.delayDisplay('Showing widget')

        webWidget.evalJS("""
        const paragraph = document.createElement('p');
        paragraph.innerText = 'Hello from Cjyx!';
        document.body.appendChild(paragraph);
    """)
        self.delayDisplay('Cjyx should be saying hello!')

        #
        # Test javascript evaluation + use of "evalResult()" signal
        #
        webWidget.connect("evalResult(QString,QString)", self.onEvalResult)

        self.delayDisplay('Cjyx setting a javascript value')

        webWidget.evalJS("const valueFromCjyx = 42;")
        webWidget.evalJS("valueFromCjyx;")

        iteration = 0
        while not self.gotResponse and iteration < 3:
            # Specify an explicit delay to ensure async execution by the
            # webengine has completed.
            self.delayDisplay('Waiting for response...', msec=500)
            iteration += 1
        webWidget.disconnect("evalResult(QString,QString)", self.onEvalResult)

        if not self.gotResponse:
            raise RuntimeError("Never got response from evalJS")

        if not self.gotCorrectResponse:
            raise AssertionError("Did not get back expected result!")

        #
        # Test python evaluation from javascript
        #
        self.delayDisplay('Call a python method')

        cjyx.app.settings().setValue("WebEngine/AllowPythonExecution", ctk.ctkMessageBox.AcceptRole)

        webWidget.evalJS(r"""
        let pythonCode = "dialog = qt.QInputDialog(cjyx.util.mainWindow())\n";
        pythonCode += "dialog.setLabelText('hello')\n";
        pythonCode += "dialog.open()\n";
        pythonCode += "qt.QTimer.singleShot(1000, dialog.close)\n";

        window.cjyxPython.evalPython(pythonCode);
    """)

        self.delayDisplay('Test access to python via js', msec=500)

        if hasattr(cjyx.modules, 'cjyxPythonValueFromJS'):
            del cjyx.modules.cjyxPythonValueFromJS

        webWidget.evalJS("""
        window.cjyxPython.evalPython("cjyx.modules.cjyxPythonValueFromJS = 42");
    """)

        iteration = 0
        while iteration < 3 and not hasattr(cjyx.modules, 'cjyxPythonValueFromJS'):
            # Specify an explicit delay to ensure async execution by the
            # webengine has completed.
            self.delayDisplay('Waiting for python value from JS...', msec=500)
            iteration += 1

        if iteration >= 3:
            raise RuntimeError("Couldn't get python value back from JS")

        self.delayDisplay('Value of %d received via javascipt' % cjyx.modules.cjyxPythonValueFromJS)

        del cjyx.modules.cjyxPythonValueFromJS

        self.delayDisplay('Test passed!')

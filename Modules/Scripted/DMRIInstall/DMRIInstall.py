import textwrap

import ctk
import qt

import cjyx
from cjyx.ScriptedLoadableModule import *


#
# DMRIInstall
#

class DMRIInstall(ScriptedLoadableModule):
    """
    """

    helpText = textwrap.dedent(
        """
  The CjyxDMRI extension provides diffusion-related tools including:

  <ul>
    <li> Diffusion Tensor Estimation</li>
    <li>Tractography Display</li>
    <li>Tractography Seeding</li>
    <li>Fiber Tract Measurement</li>
  </ul>
  <br>
  <br>
  For more information, please visit:
  <br>
  <br>
  &nbsp;&nbsp; <a href="https://dmri.slicer.org">https://dmri.slicer.org</a>
  <br>
  <br>
  Questions are welcome on the Cjyx forum:
  <br>
  <br>
  &nbsp;&nbsp; <a href="https://discourse.slicer.org">https://discourse.slicer.org</a><br><br>
  """)

    errorText = textwrap.dedent(
        """
  <h5 style="color:red">The CjyxDMRI extension is currently unavailable.</h5><br>
  Please try a manual installation via the Extensions Manager,
  and contact the Cjyx forum at:<br><br>

  &nbsp;&nbsp;<a href="https://discourse.slicer.org">https://discourse.slicer.org</a><br><br>

  With the following information:<br>
  Cjyx version: {builddate}<br>
  Cjyx revision: {revision}<br>
  Platform: {platform}
  """).format(builddate=cjyx.app.applicationVersion,
              revision=cjyx.app.repositoryRevision,
              platform=cjyx.app.platform)

    def __init__(self, parent):

        # Hide this module if CjyxDMRI is already installed
        model = cjyx.app.extensionsManagerModel()
        if model.isExtensionInstalled("CjyxDMRI"):
            parent.hidden = True

        ScriptedLoadableModule.__init__(self, parent)

        self.parent.categories = ["Diffusion"]
        self.parent.title = "Install Cjyx Diffusion Tools (CjyxDMRI)"
        self.parent.dependencies = []
        self.parent.contributors = ["Isaiah Norton (BWH), Lauren O'Donnell (BWH)"]
        self.parent.helpText = DMRIInstall.helpText
        self.parent.helpText += self.getDefaultModuleDocumentationLink()
        self.parent.acknowledgementText = textwrap.dedent(
            """
    CjyxDMRI supported by NIH NCI ITCR U01CA199459 (Open Source Diffusion MRI
    Technology For Brain Cancer Research), and made possible by NA-MIC, NAC,
    BIRN, NCIGT, and the Cjyx Community.
    """)


class DMRIInstallWidget(ScriptedLoadableModuleWidget):
    """Uses ScriptedLoadableModuleWidget base class, available at:
    https://github.com/Slicer/Slicer/blob/master/Base/Python/slicer/ScriptedLoadableModule.py
    """

    def setup(self):
        ScriptedLoadableModuleWidget.setup(self)

        self.textBox = ctk.ctkFittedTextBrowser()
        self.textBox.setOpenExternalLinks(True)  # Open links in default browser
        self.textBox.setHtml(DMRIInstall.helpText)
        self.parent.layout().addWidget(self.textBox)

        #
        # Apply Button
        #
        self.applyButton = qt.QPushButton("Install CjyxDMRI")
        self.applyButton.toolTip = 'Installs the "CjyxDMRI" extension from the Diffusion category.'
        self.applyButton.icon = qt.QIcon(":/Icons/ExtensionDefaultIcon.png")
        self.applyButton.enabled = True
        self.applyButton.connect('clicked()', self.onApply)
        self.parent.layout().addWidget(self.applyButton)

        self.parent.layout().addStretch(1)

    def onError(self):
        self.applyButton.enabled = False
        self.textBox.setHtml(DMRIInstall.errorText)
        return

    def onApply(self):
        emm = cjyx.app.extensionsManagerModel()

        if emm.isExtensionInstalled("CjyxDMRI"):
            self.textBox.setHtml("<h4>CjyxDMRI is already installed.<h4>")
            self.applyButton.enabled = False
            return

        md = emm.retrieveExtensionMetadataByName("CjyxDMRI")

        if not md or 'extension_id' not in md:
            return self.onError()

        if emm.downloadAndInstallExtension(md['extension_id']):
            cjyx.app.confirmRestart("Restart to complete CjyxDMRI installation?")
        else:
            self.onError()

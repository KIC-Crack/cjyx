import unittest
import cjyx
import os
import shutil


class CjyxUtilSaveTests(unittest.TestCase):

    def setUp(self):
        for extension in ['nrrd', 'mrml', 'mrb']:
            try:
                os.remove(cjyx.app.temporaryPath + '/CjyxUtilSaveTests.' + extension)
            except OSError:
                pass
        shutil.rmtree(cjyx.app.temporaryPath + '/CjyxUtilSaveTests', True)

    def test_saveNode(self):
        node = cjyx.util.getNode('MR-head')
        filename = cjyx.app.temporaryPath + '/CjyxUtilSaveTests.nrrd'
        self.assertTrue(cjyx.util.saveNode(node, filename))
        self.assertTrue(os.path.exists(filename))

    def test_saveSceneAsMRMLFile(self):
        filename = cjyx.app.temporaryPath + '/CjyxUtilSaveTests.mrml'
        self.assertTrue(cjyx.util.saveScene(filename))
        self.assertTrue(os.path.exists(filename))

    def test_saveSceneAsMRB(self):
        filename = cjyx.app.temporaryPath + '/CjyxUtilSaveTests.mrb'
        self.assertTrue(cjyx.util.saveScene(filename))
        self.assertTrue(os.path.exists(filename))

    def test_saveSceneAsDirectory(self):
        """Execution of 'test_saveNode' implies that the filename associated
        MR-head storage node is set to 'CjyxUtilSaveTests.nrrd'
        """
        filename = cjyx.app.temporaryPath + '/CjyxUtilSaveTests'
        self.assertTrue(cjyx.util.saveScene(filename))
        self.assertTrue(os.path.exists(filename))
        self.assertTrue(os.path.exists(filename + '/CjyxUtilSaveTests.mrml'))
        self.assertTrue(os.path.exists(filename + '/Data/CjyxUtilSaveTests.nrrd'))

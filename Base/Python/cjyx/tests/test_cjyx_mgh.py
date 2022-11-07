import unittest
import cjyx
import os
import shutil


class CjyxUtilLoadSaveMGHTests(unittest.TestCase):

    def setUp(self):
        for MGHFileNames in ['MGH_T1.mgz', 'MGH_T1_longname.mgh.gz', 'MGH_T1_uncompressed.mgz']:
            try:
                os.remove(os.path.join(cjyx.app.temporaryPath, MGHFileNames))
            except OSError:
                pass
        shutil.rmtree(os.path.join(cjyx.app.temporaryPath, 'CjyxUtilLoadSaveMGHTests'), True)

    def test_saveShortCompressedNode(self):
        node = cjyx.util.getNode('T1')
        filename = os.path.join(cjyx.app.temporaryPath, 'MGH_T1.mgz')
        self.assertTrue(cjyx.util.saveNode(node, filename))
        self.assertTrue(os.path.exists(filename))

    def test_saveUnCompressedNode(self):
        node = cjyx.util.getNode('T1_uncompressed')
        filename = os.path.join(cjyx.app.temporaryPath, 'MGH_T1_uncompressed.mgh')
        self.assertTrue(cjyx.util.saveNode(node, filename))
        self.assertTrue(os.path.exists(filename))

    def test_saveLongCompressedNode(self):
        node = cjyx.util.getNode('T1_longname')
        filename = os.path.join(cjyx.app.temporaryPath, 'MGH_T1_longname.mgh.gz')
        self.assertTrue(cjyx.util.saveNode(node, filename))
        self.assertTrue(os.path.exists(filename))

import unittest
import cjyx
import os
import shutil


class CjyxUtilLoadSaveMINCTests(unittest.TestCase):

    def setUp(self):
        for MINCFileNames in ['MINC_pd_z-_float_xyz.mnc']:
            try:
                os.remove(os.path.join(cjyx.app.temporaryPath, MINCFileNames))
            except OSError:
                pass
        shutil.rmtree(os.path.join(cjyx.app.temporaryPath, 'CjyxUtilLoadSaveMINCTests'), True)

    def test_saveMINCNode(self):
        node = cjyx.util.getNode('pd_z-_float_xyz')
        filename = os.path.join(cjyx.app.temporaryPath, 'MINC_pd_z-_float_xyz.mnc')
        self.assertTrue(cjyx.util.saveNode(node, filename))
        self.assertTrue(os.path.exists(filename))

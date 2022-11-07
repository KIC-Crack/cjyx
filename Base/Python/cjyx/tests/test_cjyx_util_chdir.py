import os
import unittest
import cjyx


class CjyxUtilChdirTests(unittest.TestCase):
    """Available in Python 3.11 as ``TestChdir`` found in ``Lib/test/test_contextlib.py``
    and adapted from https://github.com/python/cpython/pull/28271
    """
    def test_simple(self):
        old_cwd = os.getcwd()
        target = os.path.realpath(os.path.join(os.path.dirname(__file__), "../../cjyx"))
        self.assertNotEqual(old_cwd, target)

        with cjyx.util.chdir(target):
            self.assertEqual(os.getcwd(), target)
        self.assertEqual(os.getcwd(), old_cwd)

    def test_reentrant(self):
        old_cwd = os.getcwd()
        target1 = os.path.realpath(os.path.join(os.path.dirname(__file__), "../../cjyx"))
        target2 = os.path.realpath(os.path.join(os.path.dirname(__file__), "../../tests"))
        self.assertNotIn(old_cwd, (target1, target2))
        chdir1, chdir2 = cjyx.util.chdir(target1), cjyx.util.chdir(target2)

        with chdir1:
            self.assertEqual(os.getcwd(), target1)
            with chdir2:
                self.assertEqual(os.getcwd(), target2)
                with chdir1:
                    self.assertEqual(os.getcwd(), target1)
                self.assertEqual(os.getcwd(), target2)
            self.assertEqual(os.getcwd(), target1)
        self.assertEqual(os.getcwd(), old_cwd)

    def test_exception(self):
        old_cwd = os.getcwd()
        target = os.path.realpath(os.path.join(os.path.dirname(__file__), "../../cjyx"))
        self.assertNotEqual(old_cwd, target)

        try:
            with cjyx.util.chdir(target):
                self.assertEqual(os.getcwd(), target)
                raise RuntimeError("boom")
        except RuntimeError as re:
            self.assertEqual(str(re), "boom")
        self.assertEqual(os.getcwd(), old_cwd)

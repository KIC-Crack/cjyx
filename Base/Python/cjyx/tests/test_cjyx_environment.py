import qt
import cjyx
import unittest


class CjyxEnvironmentTests(unittest.TestCase):

    def setUp(self):
        pass

    def test_cjyx_util_startupEnvironment(self):
        startupEnv = cjyx.util.startupEnvironment()
        assert isinstance(startupEnv, dict)
        assert "PATH" not in startupEnv or "Cjyx-build" not in startupEnv["PATH"]

    def test_cjyx_app_startupEnvironment(self):
        startupEnv = cjyx.app.startupEnvironment()
        assert isinstance(startupEnv, qt.QProcessEnvironment)
        assert "Cjyx-build" not in startupEnv.value("PATH", "")

    def test_cjyx_app_environment(self):
        env = cjyx.app.environment()
        assert isinstance(env, qt.QProcessEnvironment)
        assert "Cjyx-build" in env.value("PATH")

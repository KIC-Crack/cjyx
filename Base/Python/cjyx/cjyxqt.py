import logging
import sys

import ctk
import qt
import vtk  # noqa: F401

import cjyx
from cjyx.util import *

# HACK Ideally constant from vtkCjyxConfigure should be wrapped,
#      that way the following try/except could be avoided.
try:
    import cjyx.cli
except:
    pass


# -----------------------------------------------------------------------------
class _LogReverseLevelFilter(logging.Filter):
    """
    Rejects log records that are at or above the specified level
    """

    def __init__(self, levelLimit):
        self._levelLimit = levelLimit

    def filter(self, record):
        return record.levelno < self._levelLimit


# -----------------------------------------------------------------------------
class CjyxApplicationLogHandler(logging.Handler):
    """
    Writes logging records to Cjyx application log.
    """

    def __init__(self):
        logging.Handler.__init__(self)
        if hasattr(ctk, 'ctkErrorLogLevel'):
            self.pythonToCtkLevelConverter = {
                logging.DEBUG: ctk.ctkErrorLogLevel.Debug,
                logging.INFO: ctk.ctkErrorLogLevel.Info,
                logging.WARNING: ctk.ctkErrorLogLevel.Warning,
                logging.ERROR: ctk.ctkErrorLogLevel.Error}
        self.origin = "Python"
        self.category = "Python"

    def emit(self, record):
        try:
            msg = self.format(record)
            context = ctk.ctkErrorLogContext()
            context.setCategory(self.category)
            context.setLine(record.lineno)
            context.setFile(record.pathname)
            context.setFunction(record.funcName)
            context.setMessage(msg)
            threadId = f"{record.threadName}({record.thread})"
            cjyx.app.errorLogModel().addEntry(qt.QDateTime.currentDateTime(), threadId,
                                                self.pythonToCtkLevelConverter[record.levelno], self.origin, context, msg)
        except:
            self.handleError(record)


# -----------------------------------------------------------------------------
def initLogging(logger):
    """
    Initialize logging by creating log handlers and setting default log level.
    """

    # Prints debug messages to Cjyx application log.
    # Only debug level messages are logged this way, as higher level messages are printed on console
    # and all console outputs are sent automatically to the application log anyway.
    applicationLogHandler = CjyxApplicationLogHandler()
    applicationLogHandler.setLevel(logging.DEBUG)
    # We could filter out messages at INFO level or above (as they will be printed on the console anyway) by adding
    # applicationLogHandler.addFilter(_LogReverseLevelFilter(logging.INFO))
    # but then we would not log file name and line number of info, warning, and error level messages.
    applicationLogHandler.setFormatter(logging.Formatter('%(message)s'))
    logger.addHandler(applicationLogHandler)

    # Prints info message to stdout (anything on stdout will also show up in the application log)
    consoleInfoHandler = logging.StreamHandler(sys.stdout)
    consoleInfoHandler.setLevel(logging.INFO)
    # Filter messages at WARNING level or above (they will be printed on stderr)
    consoleInfoHandler.addFilter(_LogReverseLevelFilter(logging.WARNING))
    logger.addHandler(consoleInfoHandler)

    # Prints error and warning messages to stderr (anything on stderr will also show it in the application log)
    consoleErrorHandler = logging.StreamHandler(sys.stderr)
    consoleErrorHandler.setLevel(logging.WARNING)
    logger.addHandler(consoleErrorHandler)

    # Log debug messages from scripts by default, as they are useful for troubleshooting with users
    logger.setLevel(logging.DEBUG)


# -----------------------------------------------------------------------------
# Set up the root logger
#
# We initialize the root logger because if somebody just called logging.debug(),
# logging.info(), etc. then it would create a default root logger with default settings
# that do not work nicely in Cjyx (prints everything in console, messages are
# not sent to the application log, etc).

initLogging(logging.getLogger())


def getCjyxRCFileName():
    """Return application startup file (Cjyx resource script) file name.
    If a .cjyxrc.py file is found in cjyx.app.cjyxHome folder then that will be used.
    If that is not found then the path defined in CJYXRC environment variable will be used.
    If that environment variable is not specified then .cjyxrc.py in the user's home folder
    will be used ('~/.cjyxrc.py')."""
    import os
    rcfile = os.path.join(cjyx.app.cjyxHome, ".cjyxrc.py")
    if not os.path.exists(rcfile):
        if 'CJYXRC' in os.environ:
            rcfile = os.environ['CJYXRC']
        else:
            rcfile = os.path.expanduser('~/.cjyxrc.py')
    rcfile = rcfile.replace('\\', '/')  # make slashed consistent on Windows
    return rcfile


# -----------------------------------------------------------------------------
#
# loadCjyxRCFile - Let's not add this function to 'cjyx.util' so that
# the global dictionary of the main context is passed to exec().
#

def loadCjyxRCFile():
    """If it exists, execute cjyx resource script"""
    import os
    rcfile = getCjyxRCFileName()
    if os.path.isfile(rcfile):
        print('Loading Cjyx RC file [%s]' % (rcfile))
        exec(open(rcfile).read(), globals())


# -----------------------------------------------------------------------------
#
# Internal
#

class _Internal:

    def __init__(self):

        # Set attribute 'cjyx.app'
        setattr(cjyx, 'app', _qCjyxCoreApplicationInstance)

        # Listen factory and module manager to update cjyx.{modules, moduleNames} when appropriate
        moduleManager = cjyx.app.moduleManager()

        # If the qCjyxApplication is only minimally initialized, the factoryManager
        # does *NOT* exist.
        # This would be the case if, for example, a commandline module wants to
        # use qCjyxApplication for tcl access but without all the managers.
        # Note: This is not the default behavior.
        if hasattr(moduleManager, 'factoryManager'):
            factoryManager = moduleManager.factoryManager()
            factoryManager.connect('modulesRegistered(QStringList)', self.setCjyxModuleNames)
            moduleManager.connect('moduleLoaded(QString)', self.setCjyxModules)
            moduleManager.connect('moduleAboutToBeUnloaded(QString)', self.unsetCjyxModule)

        # Retrieve current instance of the scene and set 'cjyx.mrmlScene'
        setattr(cjyx, 'mrmlScene', cjyx.app.mrmlScene())

        # HACK - Since qt.QTimer.singleShot is both a property and a static method, the property
        # is wrapped in python and prevent the call to the convenient static method having
        # the same name. To fix the problem, let's overwrite it's value.
        # Ideally this should be fixed in PythonQt itself.
        def _singleShot(msec, receiverOrCallable, member=None):
            """Calls either a python function or a slot after a given time interval."""
            # Add 'moduleManager' as parent to prevent the premature destruction of the timer.
            # Doing so, we ensure that the QTimer will be deleted before PythonQt is cleanup.
            # Indeed, the moduleManager is destroyed before the pythonManager.
            timer = qt.QTimer(cjyx.app.moduleManager())
            timer.setSingleShot(True)
            if callable(receiverOrCallable):
                timer.connect("timeout()", receiverOrCallable)
            else:
                timer.connect("timeout()", receiverOrCallable, member)
            timer.start(msec)

        qt.QTimer.singleShot = staticmethod(_singleShot)

    def setCjyxModuleNames(self, moduleNames):
        """Add module names as attributes of module cjyx.moduleNames"""
        for name in moduleNames:
            setattr(cjyx.moduleNames, name, name)
            # HACK For backward compatibility with ITKv3, map "dwiconvert" module name to "dicomtonrrdconverter"
            if name == 'DWIConvert':
                setattr(cjyx.moduleNames, 'DicomToNrrdConverter', name)

    def setCjyxModules(self, moduleName):
        """Add modules as attributes of module cjyx.modules"""
        moduleManager = cjyx.app.moduleManager()
        setattr(cjyx.modules, moduleName.lower(), moduleManager.module(moduleName))
        # HACK For backward compatibility with ITKv3, map "dicomtonrrdconverter" module to "dwiconvert"
        if moduleName == 'DWIConvert':
            setattr(cjyx.modules, 'dicomtonrrdconverter', moduleManager.module(moduleName))

    def unsetCjyxModule(self, moduleName):
        """Remove attribute from ``cjyx.modules``
        """
        if hasattr(cjyx.modules, moduleName + "Instance"):
            delattr(cjyx.modules, moduleName + "Instance")
        if hasattr(cjyx.modules, moduleName + "Widget"):
            delattr(cjyx.modules, moduleName + "Widget")
        if hasattr(cjyx.moduleNames, moduleName):
            delattr(cjyx.moduleNames, moduleName)
        delattr(cjyx.modules, moduleName.lower())
        try:
            cjyx.selfTests
        except AttributeError:
            cjyx.selfTests = {}
        if moduleName in cjyx.selfTests:
            del cjyx.selfTests[moduleName]


_internalInstance = _Internal()

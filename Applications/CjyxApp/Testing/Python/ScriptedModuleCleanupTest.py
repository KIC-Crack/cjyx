#!/usr/bin/env python

#
#  Program: 3D Cjyx
#
#  Copyright (c) Kitware Inc.
#
#  See COPYRIGHT.txt
#  or http://www.slicer.org/copyright/copyright.txt for details.
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#
#  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
#  and was partially funded by NIH grant 1R01EB021391
#

import argparse
import os
import tempfile

from CjyxAppTesting import (
    EXIT_FAILURE,
    EXIT_SUCCESS,
    run,
)

"""
This test verifies that an exception raised in scripted module widget cleanup
function leads to an failure exit code.

It uses an output file for communication because on Windows,
standard output is not always enabled.

Usage:
    ScriptedModuleCleanupTest.py /path/to/Cjyx [--with-testing]
"""


def check_exit_code(cjyx_executable, testing_enabled=True, debug=False):
    """
    If debug is set to True:
      * display the path of the expected test output file
      * avoid deleting the created temporary directory
    """

    temporaryModuleDirPath = tempfile.mkdtemp().replace('\\', '/')
    try:
        # Copy helper module that creates a file when startup completed event is received
        currentDirPath = os.path.dirname(__file__).replace('\\', '/')
        from shutil import copyfile
        copyfile(currentDirPath + '/ScriptedModuleCleanupTestHelperModule.py',
                 temporaryModuleDirPath + '/ModuleCleanup.py')

        common_arguments = [
            '--no-splash',
            '--disable-builtin-modules',
            '--additional-module-path', temporaryModuleDirPath,
            '--python-code', 'cjyx.util.selectModule("ModuleCleanup")'
        ]

        test_output_file = temporaryModuleDirPath + "/ModuleCleanupTest.out"
        os.environ['CJYX_MODULE_CLEANUP_TEST_OUTPUT'] = test_output_file
        if debug:
            print("CJYX_MODULE_CLEANUP_TEST_OUTPUT=%s" % test_output_file)

        # Test
        args = list(common_arguments)
        if testing_enabled:
            args.append('--testing')
        else:
            args.append('--exit-after-startup')
        (returnCode, stdout, stderr) = run(cjyx_executable, args)

        assert(os.path.isfile(test_output_file))

        if testing_enabled:
            assert(returnCode == EXIT_FAILURE)
        else:
            assert(returnCode == EXIT_SUCCESS)

    finally:
        if not debug:
            import shutil
            shutil.rmtree(temporaryModuleDirPath)


if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("/path/to/Cjyx")
    parser.add_argument('--with-testing', dest='testing_enabled', action='store_true')
    args = parser.parse_args()

    cjyx_executable = os.path.expanduser(getattr(args, "/path/to/Cjyx"))
    check_exit_code(cjyx_executable, testing_enabled=args.testing_enabled)

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
#  and was partially funded by NIH grant 1U24CA194354-01
#

import os
import sys

from CjyxAppTesting import *

"""
Usage:
    CjyxOptionDisableSettingsTest.py /path/to/Cjyx
"""


def checkUserSettings(cjyx_executable, common_args, keep_temporary_settings=False):
    # Add a user setting
    args = list(common_args)
    args.extend(['--python-code',
                 'cjyx.app.userSettings().setValue("foo", "bar"); print("foo: %s" % cjyx.app.userSettings().value("foo"))'])
    assert runCjyxAndExit(cjyx_executable, args)[0] == EXIT_SUCCESS
    print("=> ok\n")

    # User settings previously added should:
    #  * NOT be set by detault
    #  * be set if '--keep-temporary-settings' is provided
    args = list(common_args)
    condition = 'is not None'
    error = "Setting foo should NOT be set"
    if keep_temporary_settings:
        args.append('--keep-temporary-settings')
        condition = '!= "bar"'
        error = "Setting foo should be set to bar"
    args.extend(['--python-code',
                 'if cjyx.app.userSettings().value("foo") ' + condition + ': raise Exception("' + error + '.")'])
    assert runCjyxAndExit(cjyx_executable, args)[0] == EXIT_SUCCESS
    print("=> ok\n")


def checkRevisionUserSettings(cjyx_executable, common_args, keep_temporary_settings=False):
    # Add a user revision setting
    args = list(common_args)
    args.extend(['--python-code',
                 'cjyx.app.revisionUserSettings().setValue("foo", "bar"); print("foo: %s" % cjyx.app.revisionUserSettings().value("foo"))'])
    assert runCjyxAndExit(cjyx_executable, args)[0] == EXIT_SUCCESS
    print("=> ok\n")

    # User revision settings previously added should:
    #  * NOT be set by detault
    #  * be set if '--keep-temporary-settings' is provided
    args = list(common_args)
    condition = 'is not None'
    error = "Setting foo should NOT be set"
    if keep_temporary_settings:
        args.append('--keep-temporary-settings')
        condition = '!= "bar"'
        error = "Setting foo should be set to bar"
    args.extend(['--python-code',
                 'if cjyx.app.revisionUserSettings().value("foo") ' + condition + ': raise Exception("' + error + '.")'])
    assert runCjyxAndExit(cjyx_executable, args)[0] == EXIT_SUCCESS
    print("=> ok\n")


def checkKeepTemporarySettingsWithoutDisableSettingsDisplayWarning(cjyx_executable, common_args):
    args = list(common_args)
    args.remove('--disable-settings')
    args.extend(['--keep-temporary-settings'])
    (returnCode, stdout, stderr) = runCjyxAndExit(cjyx_executable, args)
    expectedMessage = "Argument '--keep-temporary-settings' requires '--settings-disabled' to be specified."
    if expectedMessage not in stderr:
        print(f"=> return code{returnCode}\n")
        raise Exception(f"Warning [{expectedMessage}] not found in stderr [{stderr}]")
    assert returnCode == EXIT_SUCCESS
    print("=> ok\n")


if __name__ == '__main__':

    if len(sys.argv) != 2:
        print(os.path.basename(sys.argv[0]) + " /path/to/Cjyx")
        exit(EXIT_FAILURE)

    cjyx_executable = os.path.expanduser(sys.argv[1])
    common_args = ['--disable-settings', '--disable-modules', '--no-main-window']

    checkUserSettings(cjyx_executable, common_args)
    checkRevisionUserSettings(cjyx_executable, common_args)

    checkUserSettings(cjyx_executable, common_args, keep_temporary_settings=True)
    checkRevisionUserSettings(cjyx_executable, common_args, keep_temporary_settings=True)

    checkKeepTemporarySettingsWithoutDisableSettingsDisplayWarning(cjyx_executable, common_args)

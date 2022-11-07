# C++ debugging with CodeLite

[CodeLite](https://codelite.org/) is a relatively lightweight, cross-platform IDE.

## Configure build

Right-click on project name and select Settings, then Customize

- Enable Custom Build: check
- Working Directory: enter path to Cjyx-build, for example `~/Cjyx-SuperBuild-Debug/Cjyx-build`
- For target `Build`: enter `make`

To configure the binary for the Run command, set Program: `~/Cjyx-SuperBuild-Debug/Cjyx-build/Cjyx` under the General tab.

## Configure debugger

This requires the use of a wrapper script, as [detailed here](linuxcpp.md#gdb-debug-by-using-exec-wrapper).

After setting up the wrapper script (`WrapCjyx` below), change the following options:

Under Settings->General:

- Program: `~/Cjyx-SuperBuild-Debug/Cjyx-build/WrapCjyx`
- Working folder: `~/Cjyx-SuperBuild-Debug/Cjyx-build`

    ![](https://github.com/Slicer/Slicer/releases/download/docs-resources/debugging_codelite_1.png)

Under Settings->Debugger

- "Enter here any commands passed to debugger on startup:"

    ```txt
    set exec-wrapper `~/Cjyx-SuperBuild-Debug/Cjyx-build/WrapCjyx`
    exec-file `~/Cjyx-SuperBuild-Debug/Cjyx-build/bin/CjyxApp-real`
    ```

    ![](https://github.com/Slicer/Slicer/releases/download/docs-resources/debugging_codelite_2.png)

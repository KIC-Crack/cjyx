# C++ debugging with Visual Studio Code

Visual Studio Code is a cross-platform IDE that can be used for debugging C++ and Python code.


:::{tip}

For debugging on Windows, Visual Studio is recommended as it has much better performance and many more features than Visual Studio Code.

:::

## Prerequisites

- Build 3D Cjyx in Debug mode, outside of Visual Studio Code, by following the [build instructions](../build_instructions/overview.md).
- Install Visual Studio Code
- Install the `C/C++` extension in Visual Studio Code

````{note}

Visual Studio Code uses GDB for debugging on Linux, which requires Python. However, Python that is linked into GDB is not the same as Cjyx's Python, which may cause issues. If GDB does not start because `_sysconfigdata__linux_x86_64-linux-gnu.py` file is missing then Cjyx's sysconfigdata file must be copied to the expected filename. For example:

```bash
cd ~/D/Cjyx-SuperBuild-Debug/python-install/lib/python3.6/
cp _sysconfigdata_m_linux2_.py _sysconfigdata__linux_x86_64-linux-gnu.py
```

````

## Running the debugger on Linux

1. From a terminal, launch Visual Studio Code through the Cjyx launcher to setup environment to set up the directory paths necessary for running the Cjyx application. For example:

    ```bash
    cd ~/D/Cjyx-SuperBuild-Debug/Cjyx-build
    ./Cjyx --launch code
    ```

2. Set up settings.json. For example:

    ```json
    {
        "launch": {
            "configurations": [
                      {
                        "name": "Cjyx debug",
                        "type": "cppdbg",
                        "request": "launch",
                        "program": "/home/perklab/D/Cjyx-SuperBuild-Debug/Cjyx-build/bin/CjyxApp-real",
                        "args": [],
                        "stopAtEntry": false,
                        "cwd": ".",
                        "environment": [],
                        "externalConsole": false,
                        "MIMode": "gdb",
                        "setupCommands": [
                          {
                            "description": "Enable pretty-printing for gdb",
                            "text": "-enable-pretty-printing",
                            "ignoreFailures": true
                          }
                        ],
                        "miDebuggerPath": "/usr/bin/gdb"
                      }
            ]
        }
    }
    ```

3. Choose Run / Start debugging in the menu to start Cjyx application with the debugger attached

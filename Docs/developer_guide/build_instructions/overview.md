#  Overview

Building Cjyx is the process of obtaining a copy of the source code of the project and use tools, such as compilers, project generators and build systems, to create binary libraries and executables. Cjyx documentation is also generated in this process.

Users of Cjyx application and extensions do not need to build the application and they can download and install pre-built packages instead. Python scripting and development of new Cjyx modules in Python does not require building the application either. Only software developers interested in developing Cjyx [modules](../../user_guide/modules/index.md) in C++ language or contributing to the development of Cjyx core must build the application.

Cjyx is based on a *superbuild* architecture. This means that the in the building process, most of the dependencies of Cjyx will be downloaded in local directories (within the Cjyx build directory) and will be configured, built and installed locally, before Cjyx itself is built. This helps reducing the complexity for developers.

As Cjyx is continuously developed, build instructions may change, too. Therefore, it is recommended to use build instructions that have the same version as the source code.

## Extensions for developer builds of Cjyx

In general, Cjyx versions that a developer builds on his own computer are not expected to work with extensions in the Extensions Server.

Often the Extensions Manager does not show any extensions for a developer build. The reason is that extensions are only built for one Cjyx version a day, and so there are many Cjyx versions for that no extensions are available.

Even if the Extensions Manager is not empty, the listed extensions are not expected to be compatible with developer builds, created on a different computer, in a different build environment or build options, due to porential ABI incompatibility issues.

If a developer builds the Cjyx application then it is expected that the developer will also build all the extension he needs. Building all the extensions after Cjyx build is completed [is a simple one-line command](../extensions.md#build-test-and-package). It is also possible to just [build selected extensions](../extensions.md#build-an-extension).

## Custom builds

Customized editions of Cjyx can be generated without changing Cjyx source code, just by modifying CMake variables:

- `CjyxApp_APPLICATION_NAME`: Custom application name to be used, instead of default "Cjyx". The name is used in installation package name, window title bar, etc.
- `Cjyx_DISCLAIMER_AT_STARTUP`: String that is displayed to the user after first startup of Cjyx after installation (disclaimer, welcome message, etc).
- `Cjyx_DEFAULT_HOME_MODULE`: Module name that is activated automatically on application start.
- `Cjyx_DEFAULT_FAVORITE_MODULES`: Modules that will be added to the toolbar by default for easy access. List contains module names, separated by space character.
- `Cjyx_CLIMODULES_DISABLED`: Built-in CLI modules that will be removed from the application. List contains module names, separated by semicolon character.
- `Cjyx_QTLOADABLEMODULES_DISABLED`: Built-in Qt loadable modules that will be removed from the application. List contains module names, separated by semicolon character.
- `Cjyx_QTSCRIPTEDMODULES_DISABLED`: Built-in scripted loadable modules that will be removed from the application. List contains module names, separated by semicolon character.
- `Cjyx_USE_PYTHONQT_WITH_OPENSSL`: enable/disable building the application with SSL support (ON/OFF)
- `Cjyx_USE_SimpleITK`: enable/disable SimpleITK support (ON/OFF)
- `Cjyx_BUILD_SimpleFilters`: enable/disable building SimpleFilters. Requires SimpleITK. (ON/OFF)
- `Cjyx_EXTENSION_SOURCE_DIRS`: Defines additional extensions that will be included in the application package as built-in modules. Full paths of extension source directories has to be specified, separated by semicolons.
- `Cjyx_BUILD_WIN32_CONSOLE_LAUNCHER`: Show/hide console (terminal window) on Windows.

Moreoptions are listed in CMake files, such as in [CjyxApplicationOptions.cmake](https://github.com/Slicer/Slicer/blob/master/CMake/SlicerApplicationOptions.cmake) and further customization is achievable by using [SlicerCustomAppTemplate](https://github.com/KitwareMedical/SlicerCustomAppTemplate) project maintained by Kitware.

# Extensions

Developers can create extensions to provide additional features to users. See an overview of extensions in the [Extensions manager page](../user_guide/extensions_manager).

## Create an extension

If you have developed a script or module that you would like to share with others then it is recommended to submit it to the Cjyx Extensions Index. Indexed extensions get listed in the Extensions Manager in Cjyx and user can install them by a few mouse clicks.

- Scan through the [user](../user_guide/extensions_manager.md) and [developer](https://www.slicer.org/wiki/Documentation/Nightly/Developers/FAQ/Extensions) extension FAQs
- Inform a community about your plans on the [Cjyx forum](https://discourse.slicer.org) to get information about potential parallel efforts (other developers may already work on a similar idea and you could join or build on each other's work), past efforts (related tools might have been available in earlier Slicer versions or in other software that you may reuse), and get early feedback from prospective users. You may also seek advice on the name of your extension and how to organize features into modules. All these can save you a lot of time in the long term.
- If you have not done already, use the [Extension Wizard module](https://www.slicer.org/wiki/Documentation/Nightly/Developers/ExtensionWizard) in Slicer to create an extension that will contain your module(s).
- If developing [C++ loadable or CLI modules](https://www.slicer.org/wiki/Documentation/Nightly/Developers/Modules) (not needed if developing in Python):
  - [build Cjyx application](build_instructions/index.md).
  - [build your extension](#build-an-extension)

## Build an extension

:::{note}

To build extensions that contain modules implemented in C++, you need to [build Cjyx from source](build_instructions/index.md) on your machine; they cannot be built against a binary download.
If developing modules in Python only, then it is not necessary to build the extension.

:::

Similarly to the building of Cjyx core, multi-configuration builds are not supported: one build tree can be only used for one build mode (Release or Debug or RelWithDebInfo or MinSizeRel). If a release and debug mode build are needed then the same source code folder can be used (e.g., `C:\D\CjyxHeart`) but a separate binary folder must be created for each build mode (e.g., `C:\D\CjyxHeart-R` and `C:\D\CjyxHeart-D` for release and debug modes).

Assuming that the source code of your extension is located in folder `MyExtension`, an extension can be built by the following steps.

### Linux and macOS

Start a terminal.

```bash
$ mkdir MyExtension-debug
$ cd MyExtension-debug
$ cmake -DCMAKE_BUILD_TYPE:STRING=Debug -DCjyx_DIR:PATH=/path/to/Cjyx-SuperBuild-Debug/Cjyx-build ../MyExtension
$ make
```

#### CMAKE_OSX_ variables

On macOS, the extension must be configured specifying `CMAKE_OSX_*` variables matching the one used to configure Cjyx: `-DCMAKE_OSX_ARCHITECTURES:STRING=x86_64 -DCMAKE_OSX_DEPLOYMENT_TARGET:STRING=/same/as/Cjyx -DCMAKE_OSX_SYSROOT:PATH=SameAsCjyx`

Instead of manually setting these variables, within your extension, including the <code>ConfigurePrerequisites</code> component before the project statement should ensure it uses the same CMAKE_OSX_* variables as Cjyx:

```txt
find_package(Cjyx COMPONENTS ConfigurePrerequisites REQUIRED)

project(Foo)

[...]

find_package(Cjyx REQUIRED)
include(${Cjyx_USE_FILE})

[...]
```

For more details, see [here](https://github.com/Slicer/Slicer/blob/6f4e2946bb129d317dfdb1116f06f5308b449044/CMake/SlicerConfig.cmake.in#L10-L38).

### Windows

Run `CMake (cmake-gui)` from the Windows Start menu.

- Select source and build directory
- Click `Configure`
- Select generator (just accept the default if you only have one compiler toolset installed)
- Choose to create build directory if asked
- The configuration is expected to display an error message due to `Cjyx_DIR` variable not specified yet.
- Specify `Cjyx_DIR` by replacing `Cjyx_DIR-NOTFOUND` by the Cjyx inner-build folder (for example `c:/D/S4D/Cjyx-build`).
- Click `Configure`. No errors should be displayed.
- Click `Generate` button.
- Click `Open project` button to open `MyExtension.sln` in Visual Studio.
- Select build configuration (Debug, Release, ...) that matches the build configuration (Release, Debug, ...) of the chosen Cjyx build.
- In the menu choose Build / Build Solution.

## Test an extension

### Run Cjyx with your custom modules

To test an extension, you need to specify location(s) where Cjyx should look for additional modules.

- If the extension is not built: add all source code folders that contain module .py files to "additional module paths" in modules section in application settings.
- If the extension is built:
  - Option A: start the application using the `CjyxWithMyExtension` executable in your build directory. This starts Cjyx, specifying additional module paths via command-line arguments.
  - Option B: specify additional module paths manually in application settings. Assuming your extension has been built into folder `MyExtension-debug`, add these module folders (if they exist) to the additional module paths in Cjyx's application settings:
    - `C:\path\to\MyExtension-debug\lib\Cjyx-4.13\qt-scripted-modules`
    - `C:\path\to\MyExtension-debug\lib\Cjyx-4.13\qt-loadable-modules\Debug` or `C:\path\to\MyExtension-debug\lib\Cjyx-4.13\qt-loadable-modules\Release` or (on systems where multi-configuration builds are not used, such as linux) simply `C:\path\to\MyExtension-debug\lib\Cjyx-4.13\qt-loadable-modules`:
    - `C:\path\to\MyExtension-debug\lib\Cjyx-4.13\cli-modules\Debug`, `C:\path\to\MyExtension-debug\lib\Cjyx-4.13\cli-modules\Release`, or `C:\path\to\MyExtension-debug\lib\Cjyx-4.13\cli-modules`

### Run automatic tests

Automatic tests of your extension can be launched by following the instructions below.

#### Linux and macOS

Start a terminal.

```bash
$ ctest -j<NUMBEROFCORES>
```

#### Windows

##### Run all tests

Open a command prompt.

```text
cd C:\path\to\MyExtension-debug
"c:\Program Files\CMake\bin\ctest.exe" -C Release -V
```

Replace `Release` with the build mode of your extension build (`Debug`, `Release`, ...).

##### To debug a test

- Launch Visual Studio from the Command Line Prompt: `C:\D\S4D\Cjyx-build\Cjyx.exe --VisualStudio --launcher-no-splash --launcher-additional-settings C:\path\to\MyExtension-debug\AdditionalLauncherSettings.ini C:\path\to\MyExtension-debug\MyExtension.sln`
- Find the project of the test you want to debug (e.g., `qCjyxMODULE_NAMEModuleGenericCxxTests`).
- Go to the project debugging properties (right-click -> Properties, then Configuration Properties / Debugging).
- In `Command Arguments`, type the name of the test you want to run (e.g., `qCjyxMODULE_NAMEModuleGenericTest`).
- If the test takes arguments, enter the arguments after the test name in `Command Arguments`.
- Set the project as the StartUp Project (right-click -> Set As StartUp Project).
- Start debugging (F5).

### Continuous integration

If you shared your extension by using the ExtensionWizard, make sure you know about the Cjyx testing dashboard:
- [Dashboard for Cjyx Stable Releases](https://slicer.cdash.org/index.php?project=SlicerStable)
- [Dashboard for Cjyx Preview Releases](https://slicer.cdash.org/index.php?project=SlicerPreview)

The dashboard will attempt to check out the source code of your extension, build, test and package it on Linux, macOS and Windows platforms.

To find your extension, use the following link replacing `CjyxMyExtension` with the name of your extension:

`https://slicer.cdash.org/index.php?project=SlicerStable&filtercount=1&showfilters=1&field1=buildname&compare1=63&value1=SlicerMyExtension`

For example, here is the link to check the status of the `CjyxDMRI` extension:

`https://slicer.cdash.org/index.php?project=SlicerStable&filtercount=1&showfilters=1&field1=buildname&compare1=63&value1=SlicerDMRI`

If you see red in any of the columns for your extension, click on the hyperlinked number of errors to see the details.

Always check the dashboard after you first introduce your extension, or after you make any changes to the code.

## Create an extension package

Assuming your extension has been built into folder `MyExtension-release` (redistributable packages must be built in release mode), this could be achieved doing:

### Linux and macOS

Start a terminal.

```bash
$ make package
```

### Windows

- Open `MyExtension.sln` in Visual Studio.
- Right-click on `PACKAGES` project, then select `Build`.

## Write documentation for an extension

Keep documentation with your extension's source code and keep it up-to-date whenever the software changes.

Add at least a README.md file in the root of the source code repository, which describes what the extension is for and how it works. Minimum information that is needed to make your extension usable is described in the [extension submission checklist](https://github.com/Slicer/ExtensionsIndex/blob/master/.github/PULL_REQUEST_TEMPLATE.md#todo-list-for-submitting-a-new-extension).

Extension documentation examples:
- [SegmentMesher](https://github.com/lassoan/SlicerSegmentMesher)
- [SequenceRegistration](https://github.com/moselhy/SlicerSequenceRegistration)
- [AI-assisted annotation client](https://github.com/NVIDIA/ai-assisted-annotation-client/tree/master/slicer-plugin)
- [CjyxDMRI](https://dmri.slicer.org/) - large extension documented using Github pages

Thumbnails to YouTube videos can be generated by using an URL of the form `https://img.youtube.com/vi/<insert-youtube-video-id-here>/0.jpg` and
adding a playback button using [this free service](https://addplaybuttontoimage.way4info.net/) (the second red arrow is recommended).

## Distribute an extension

- Upload source code of your extension to a publicly available repository. It is recommended to start the repository name with "Cjyx" (to make Cjyx extensions easier to identify) followed by your extension name (for example, "Sequences" extension is stored in "CjyxSequences" repository). However, this is not a mandatory requirement. If you have a compelling reason not to use Cjyx prefix, please make a note while making the pull request. See more requirements in the [new extension submission checklist](https://github.com/Slicer/ExtensionsIndex/blob/master/.github/PULL_REQUEST_TEMPLATE.md#todo-list-for-submitting-a-new-extension).
  - GitHub is recommended (due to large user community, free public project hosting): [join Github](https://github.com/join) and [setup Git](https://docs.github.com/get-started/quickstart/set-up-git).
- If developing an extension that contains [C++ loadable or CLI modules](https://www.slicer.org/wiki/Documentation/Nightly/Developers/Modules) (not needed if developing in Python):
  - Build the `PACKAGE` target to create a package file.
  - Test your extension by installing the created package file using the Extensions Manager.
- Complete the [extension submission checklist](https://github.com/Slicer/ExtensionsIndex/blob/master/.github/PULL_REQUEST_TEMPLATE.md#todo-list-for-submitting-a-new-extension)) then submit it to the Slicer Extensions Index:
- Submit the extension to the Extensions Index:
  - Fork ExtensionIndex repository on GitHub by clicking ''Fork'' button on the [Cjyx Extensions Index](https://github.com/Slicer/ExtensionsIndex) page
  - Create an [extension description (s4ext) file](#extension-description-file)
    - If the extension was built then you can find the automatically generated extension description in the build folder
    - If the extension was not built then create the extension description file manually, using a text editor
  - Add your .s4ext file to your forked repository: it can be done using a git client or simply by clicking ''Upload files'' button
    - To make the extension appear in the latest Cjyx Preview Release: upload the file into the `master` branch.
    - To make the extension appear in the latest Cjyx Stable Release: upload the file into the branch corresponding to the stable release version, for example: `4.10`.
  - Create a pull request: by clicking ''Create pull request'' button
  - Follow the instructions in the pull request template

## Application settings

After installing an extension, the directories are added to revision-specific settings:
- Folders containing modules bundled within an extension are added to Modules / AdditionalPaths. This ensures that libraries associated with modules are found.
- Folders containing third-party dynamic libraries, Python libraries, etc. are added to LibraryPaths, Paths, PYTHONPATH, QT_PLUGIN_PATH. This ensures that libraries associated with modules can be successfully loaded.

## Extension description file

An extension description file is a text file with `s4ext` extension allowing to specify metadata associated with an extensions.

The description file is automatically generated by the build system in the build tree of the extension from the metadata specified in the top-level `CMakeLists.txt` file in the source code of the extension. Since the description is a very simple text file, the description can be created using a text editor. It may be simpler to use a text editor to create the file if the extension only contains scripted modules, which can be developed without building the extension.

Note that the Extension manager ignores many fields of the extension description file and instead uses information specified in `CMakeLists.txt`. Therefore, when making any changes to the extension description file, it has to be done in the `CMakeLists.txt` file as well.

For superbuild-type extensions (that build their own dependencies, such as external libraries implemented in C++), it is critical that the build_subdirectory is initialized to the inner build location in s4ext. The value of this variable in `CMakeLists.txt` is not used in all places by the dashboard scripts.

### Syntax

- A metadata is specified using a keyword followed by at least one spaces and the associated value.
- Multiline values are not supported.
- Empty lines are ignored
- Lines starting with a `#` are considered comments and ignored.

The following code block illustrates how comments, metadata and associated value can be specified:

```
# This is a comment
metadataname This is the value associated with 'metadataname'

# This is an other comment
anothermetadata This is the value associated with 'anothermetadata'
```

### Supported metadata fields

```{list-table}
:header-rows: 1

* - Name
  - Description
  - Required
* - scm
  - Source code management system. Must be `git`.
  - Y
* - scmurl
  - Read-only url used to checkout the extension source code.
  - Y
* - scmrevision
  - Revision allowing to checkout the expected source code.
  - Y
* - depends
  - List of extensions required to build this extension. Specify "NA" if there are no dependency. Extension names should be separated by a single space. For example: extensionA extensionB.
  - N
* - build_subdirectory
  - Name of the inner build directory in case of superbuild based extension. Default to `.`.
  - N
* - homepage
  - URL of the web page describing the extension.
  - Y
* - contributors
  - Extension contributor specified as `Firstname1 Lastname1 ([SubOrg1, ]Org1), Firstname2 Lastname2 ([SubOrg2, ]Org2)`.
  - N
* - category
  - Extension category.
  - Y
* - iconurl
  - URL to an icon (png file format and size of 128x128 pixels is recommended).
  - N
* - description
  - One line describing what is the purpose of the extension.
  - N
* - screenshoturls
  - Space separated list of urls to images.
  - N
* - enabled
  - Specify if the extension should be enabled after its installation. Valid values are `1` (default) = enabled; `0` = disabled.
  - N
* - status
  - Give people an idea what to expect from this code. This is currently not used.
  - N
```

Note: Parameters in URLS (such as `&foo=bar`) are not supported. URL shortener services can be used if necessary.

## Extensions server

The official Cjyx extensions server, the "Extensions Catalog" is available at <https://extensions.slicer.org/>. To get a list of extensions, specify the Slicer revision and platform in the URL, for example: <https://extensions.slicer.org/catalog/All/30117/win>.

The Extensions Catalog is implemented a web application ([source code](https://github.com/KitwareMedical/slicer-extensions-webapp)), which connects
to a [Girder server](https://slicer-packages.kitware.com/#collection/5f4474d0e1d8c75dfc70547e/folder/5f4474d0e1d8c75dfc705482), a general-purpose
storage server with the Cjyx Package Manager plugin ([source code](https://github.com/girder/slicer_package_manager)), which provides a
convenient REST API for accessing Cjyx extension packages and metadata.

The "Manage extensions" tab in the Extensions Manager in Cjyx uses this REST API to get information on updates and get packages to install or update.

The extension server is designed so that organizations can set up and maintain their own extensions servers, for example to distribute
extensions for custom applications. Extensions server address can be set in the Application Settings, in the Extensions section.

Until August 2021, a Midas-based server at `https://slicer.kitware.com/midas3` was used. This server is not online anymore, as it was not feasible to perform all software updates that would have kept it secure.

## Extensions Index

The ExtensionsIndex is a repository containing a list of [extension description files](#extension-description-file) `*.s4ext` used by the Cjyx [Extensions build system](#extensions-build-system) to build, test, package and upload extensions on the [extensions server](#extensions-server).

The ExtensionsIndex is hosted on GitHub: <https://github.com/Slicer/ExtensionsIndex>

Each branch of the repository contains extension descrtiption files that corresponds to the same branch in the Cjyx repository. For example, `master` branch contains descriptions for Cjyx `master` branch, and `4.11` branch contains extension descripions for Cjyx's `4.11` branch.

Extension developers have to make sure that the extension description in each branch of the Extensions index is compatible with the corresponding Cjyx version. Extension developers often create the same branches (`master`, `4.11`, `4.13`, ...) in their repository and they specify this branch name in the extensions descriptor file.

## Extensions build system

The extensions build system allows to drive the build, test, packaging and upload of Cjyx extensions.

Using the [extensions build system source code](https://github.com/Slicer/Slicer/tree/master/Extensions/CMake), it is possible to build extensions using either manual build or dashboard-driven automatic build. The extension description files must be simply placed in a folder, the same way as they are in the Extensions Index repository.

### Build list of extensions manually

Locally building a list of extensions is a convenient way to test building of extensions and get extension packages for a custom-build Cjyx application.

Given a directory containing one or more extension description files, with the help of the extensions build system it is possible to configure and build the associated extensions specifying the following CMake options:

```{list-table}
:header-rows: 1

* - CMake variable
  - Description
* - Cjyx_DIR
  - Path to Cjyx build tree. Required.
* - Cjyx_EXTENSION_DESCRIPTION_DIR
  - Path to folder containing extension description files. Required.
* - [CMAKE_BUILD_TYPE](https://www.cmake.org/cmake/help/v2.8.8/cmake.html#variable:CMAKE_BUILD_TYPE)
  - Build type of the associated Cjyx build directory
* - CTEST_MODEL
  - By default set to `Experimental`.
    Allow to choose on which CDash track results are submitted as well as setting the submission type associated with the uploaded extension.
* - Cjyx_UPLOAD_EXTENSIONS
  - By default set to `OFF`.
    If enabled, extension builds will be submitted to Cjyx dashboard and associated packages will be uploaded to extensions server.
* - CJYX_PACKAGE_MANAGER_URL
  - Cjyx extensions server URL specifying where the extension should be uploaded. For example `https://slicer-packages.kitware.com`. source code.
    Note that this variable is expected to be set in place of `MIDAS_PACKAGE_URL`.
* - CJYX_PACKAGE_MANAGER_API_KEY
  - Token allowing to authenticate to the extensions server.
    Note that this variable is expected to be set in place of `MIDAS_PACKAGE_API_KEY` and `MIDAS_PACKAGE_EMAIL`.
```

The following folders will be used in the examples below:

| Folder                                                                               | Linux/macOS                                      | Windows  |
|--------------------------------------------------------------------------------------|--------------------------------------------------|----------|
| Cjyx source code tree (checked out from https://github.com/Slicer/Slicer.git)      | `~/Slicer`                                       | `C:\D\S4`               |
| Cjyx build tree (built by following Cjyx build instructions)                     |  `~/Cjyx-SuperBuild-Release`                   | `C:\D\S4R`              |
| List of extension description files (for example checked out from https://github.com/Slicer/ExtensionsIndex.git) |  `~/ExtensionsIndex` | `C:\D\ExtensionsIndex`  |
| Folder to store built extensions (new empty folder)                                  |  `~/ExtensionsIndex-Release`                     | `C:\D\ExtensionsIndexR` |

#### Build, test, and package

Linux and macOS:

```
cd ~/ExtensionsIndex-Release

cmake -DCjyx_DIR:PATH=~/Cjyx-SuperBuild-Release/Cjyx-build \
 -DCjyx_EXTENSION_DESCRIPTION_DIR:PATH=~/ExtensionsIndex \
 -DCMAKE_BUILD_TYPE:STRING=Release \
 ~/Cjyx/Extensions/CMake

make
```

Windows:

```
cd /d C:\D\ExtensionsIndexR

"c:\Program Files\CMake\bin\cmake.exe" -DCjyx_DIR:PATH=C:/D/S4R/Cjyx-build ^
 -DCjyx_EXTENSION_DESCRIPTION_DIR:PATH=C:/D/ExtensionsIndex ^
 -DCMAKE_BUILD_TYPE:STRING=Release ^
 C:/D/S4/Extensions/CMake

"c:\Program Files\CMake\bin\cmake.exe" --build . --config Release
```

#### Build, test, package, and upload to extensions server

Submit the configure/build/test results to the Cjyx Dashboard `Extensions-Experimental` track and upload the extension to a custom Extensions Server.

Linux and macOS:

```
cd ~/ExtensionsIndex-Release
cmake -E env \
  CJYX_PACKAGE_MANAGER_CLIENT_EXECUTABLE=/path/to/cjyx_package_manager_client \
  CJYX_PACKAGE_MANAGER_URL=https://slicer-packages.kitware.com \
  CJYX_PACKAGE_MANAGER_API_KEY=a0b012c0123d012abc01234a012345a0 \
  \
cmake -DCjyx_DIR:PATH=~/Cjyx-SuperBuild-Release/Cjyx-build \
 -DCjyx_EXTENSION_DESCRIPTION_DIR:PATH=~/ExtensionsIndex \
 -DCMAKE_BUILD_TYPE:STRING=Release \
 -DCTEST_MODEL:STRING=Experimental \
 -DCjyx_UPLOAD_EXTENSIONS:BOOL=ON \
 ~/Cjyx/Extensions/CMake

make
```

Windows:

```
cd /d C:\D\ExtensionsIndexR
"c:\Program Files\CMake\bin\cmake.exe" -E env ^
  CJYX_PACKAGE_MANAGER_CLIENT_EXECUTABLE=/path/to/cjyx_package_manager_client ^
  CJYX_PACKAGE_MANAGER_URL=https://slicer-packages.kitware.com ^
  CJYX_PACKAGE_MANAGER_API_KEY=a0b012c0123d012abc01234a012345a0 ^
  ^
"c:\Program Files\CMake\bin\cmake.exe" -DCjyx_DIR:PATH=~/Cjyx-SuperBuild-Release/Cjyx-build ^
 -DCjyx_EXTENSION_DESCRIPTION_DIR:PATH=C:/D/ExtensionsIndex ^
 -DCMAKE_BUILD_TYPE:STRING=Release ^
 -DCTEST_MODEL:STRING=Experimental ^
 -DCjyx_UPLOAD_EXTENSIONS:BOOL=ON ^
 C:/D/S4/Extensions/CMake

make
```

### Build complete Extensions Index with dashboard submission

Continuous and nightly extension dashboards are setup on the Cjyx factory machine maintained by [Kitware](https://www.kitware.com). Developers can set up similar infrastructure privately for their custom applications.

By customizing the [extension template dashboard script](https://github.com/Slicer/Slicer/blob/master/Extensions/CMake/SlicerExtensionsDashboardScript.TEMPLATE.cmake), it is possible to easily setup dashboard client submitting to [CDash](https://slicer.cdash.org/index.php?project=SlicerPreview). See example dashboard scripts that are used on official Slicer build machines [here](https://github.com/Slicer/DashboardScripts). Note that these scripts are more complex than the template to allow code reuse between different configurations, but they are tested regularly and so guaranteed to work.

## Frequently asked questions

### Can an extension contain different types of modules?

Yes. Extensions are used to package together all types of Cjyx modules.

### Should the name of the source repository match the name of the extension?

Assuming your extension is named `AwesomeTool`, generally, we suggest to name the extension repository `CjyxAwesomeTool`. Doing so will minimize confusion by clearly stating that the code base is associated with Cjyx.

We suggest to use the `Cjyx` prefix in the extension name, too, when the extension is a Cjyx interface to some third-party library (such as CjyxOpenIGTLink, CjyxElastix, CjyxANTs CjyxOpenCV).

### Where can I find the extension templates?

The module and extension templates are available in the Cjyx source tree: <https://github.com/Slicer/Slicer/tree/master/Utilities/Templates/>

Using the [Extension Wizard module](https://www.slicer.org/wiki/Documentation/Nightly/Developers/ExtensionWizard), developers can easily create a new extension without having to copy, rename and update manually every files.

### How are Superbuild extension packaged?

Extensions using the Superbuild mechanism build projects in two steps:
- First, the project dependencies are built in an outer-build directory.
- Then, the project itself is built in an inner-build directory.

Extensions can use the Superbuild (i.e., CMake's ExternalProject_Add) mechanism. However, developers have to be careful that the packaging macros clean the project before reconfiguring it. This means that if one uses the Cjyx extension packaging macros inside the inner-build directory, when packaging and uploading the extension package, the project will be reconfigured, and variables passed from the outer-build directory will be lost. If the project only depends on libraries that Cjyx builds, this is not an issue. If the project has specific dependencies that Cjyx does not compile on its own, the developer should be careful to instantiate the Cjyx extension packaging macros only in the outer-build directory. This only means that in the latter case, tests should be instantiated in the outer-build directory to allow the Cjyx extension building process to test the extension before uploading the extension and the tests results.

### How to build a custom Cjyx package with additional extensions bundled?

To build custom Cjyx versions, it is recommended to use the [Cjyx Custom Application Template](https://blog.kitware.com/slicercat-creating-custom-applications-based-on-3d-slicer/).

### Can an extension depend on other extensions?

Yes. The dependency should be specified as a list by setting the variable `EXTENSION_DEPENDS` in the extension `CMakeLists.txt` and in the `dependency` field in the extension description. If the user installs `Extension2` that depends on `Extension1` then the extension manager will install `Extension1` automatically.

If you have `ModuleA`, `ModuleB` and `ModuleC` and `ModuleA` can be used as standalone one. You could create the following extensions:

- `Extension1` containing `ModuleA`.
- `Extension2` containing `ModuleB` and `ModuleC`, depending on `Extension1`.

Add the following variable to `Extension2/CMakeLists.txt`:

```txt
set(EXTENSION_DEPENDS Extension1)
```

### How dependent extensions are configured and built?

If an ExtensionB depends on an ExtensionA, ExtensionA should be listed as dependency in the metadata of ExtensionB.

This can be done setting <code>EXTENSION_DEPENDS</code> in the <code>CMakeLists.txt</code> or by specifying <code>depends</code> field in the [[Documentation/{{documentation/version}}/Developers/Extensions/DescriptionFile|description file]].

Doing so will ensure that:

*(1) the extension build system configure the extensions in the right order
*(2) ExtensionB is configured with option <code>ExtensionA_DIR</code>.


### What are the extension specific targets: INSTALL, PACKAGE, packageupload, ...?

Cjyx extension build system provides the developer with a set of convenient targets allowing to build and upload extensions.

- `RUN_TESTS`: Locally execute all tests.
- `PACKAGE` or `package`: Locally package the extension.
- `packageupload`: Locally package and upload the extension and upload to the extensions server. Requires access privileges to the server.
- `Experimental`: Configure, build, test the extension and publish result on CDash.
- `Continuous` / `Nightly`: retrieves the latest / latest nightly revision and runs the same steps as for the `Experimental` target.

### Is --launch flag available for a MacOSX installed Cjyx.app?

On MacOSx, running Cjyx with the `--help` argument does NOT list the usual launcher related options.

```bash
 $ ./Cjyx.app/Contents/MacOS/Cjyx --help
 Usage
  Cjyx [options]

  Options
    --, --ignore-rest                     Ignores the rest of the labeled arguments following this flag. (default: false)
    -h, --help                            Display available command line arguments.
    [...]
    --version                             Displays version information and exits.
```

To provide some background information, when generating the package that will be distributed, an application bundle `Cjyx.app` is created. As explained [here](https://developer.apple.com/library/mac/#documentation/CoreFoundation/Conceptual/CFBundles/Introduction/Introduction.html), a bundle is a directory with a standardized hierarchical structure that holds executable code and the resources used by that code. It means that since all libraries contained within a bundle are referenced relatively to the location of either the CLI or the Slicer executable, the use of launcher does NOT make sens.

To help fixing-up the libraries, executables and plugins so that they reference each other in a relative way, CMake provides us with the [BundleUtilities](https://www.cmake.org/cmake/help/v2.8.8/cmake.html#module:BundleUtilities) module.

This module is used in two situations:

- Fixup of Cjyx application itself. See [CjyxCPack.cmake#L36-68](https://github.com/Slicer/Slicer/blob/master/CMake/SlicerCPack.cmake#L36-68) and [SlicerCPackBundleFixup.cmake.in](https://github.com/Slicer/Slicer/blob/master/CMake/SlicerCPackBundleFixup.cmake.in).
- Fixup of an extension package. See [CjyxExtensionCPack.cmake#L126-143](https://github.com/Slicer/Slicer/blob/master/CMake/SlicerExtensionCPack.cmake#L126-143) and [SlicerExtensionCPackBundleFixup.cmake.in](https://github.com/Slicer/Slicer/blob/master/CMake/SlicerExtensionCPackBundleFixup.cmake.in).

### How to check if an extension is built by Cjyx Extensions build system?

Sometimes it is desirable to build the same source code in two different modes: as a standalone package or as a Cjyx extension. To differentiate the two cases, the developer could check for the value of `<ExtensionName>_BUILD_CJYX_EXTENSION` CMake variable. This variable will be set to ON when the extension is built by the Cjyx Extensions build system and it is not set otherwise. See details [here](https://github.com/Slicer/Slicer/blob/0ac1d7c71e2faf6dc01262e48a9d18b93f731da4/Extensions/CMake/SlicerBlockBuildPackageAndUploadExtension.cmake#L142)

### How often extensions are uploaded on the extensions server?

Cjyx extensions are built and uploaded to the extensions server every day.

- Packages for Cjyx stable release are rebuilt and uploaded during the day (Eastern time). Results are available at <https://slicer.cdash.org/index.php?project=SlicerStable>
- Packages for the latest Cjyx Preview Release is built every night (Eastern time). Results are available at <https://slicer.cdash.org/index.php?project=SlicerPreview>

Note that packages are not updated for previous Cjyx Preview Releases. To get latest extensions for a Cjyx Preview Release, install the latest Cjyx Preview Release.

### Will an extension be uploaded if associated tests are failing?

Independently of the extension test results, if the extension could be successfully packaged, it will be uploaded on the extensions server.

### How do I associate a remote with my local extension git source directory?

- Start a terminal (or Git Bash on Windows)
- Get the associated SSH remote url. [Need help?](https://docs.github.com/get-started/getting-started-with-git/about-remote-repositories#cloning-with-ssh)
- Associate the remote URL with your local git source tree

    ```bash
    git remote add origin https://github.com/<username>/MyExtension
    ```

### Which remote name is expected for extension git checkout?

When packaging an extension and generating the associated extension description file, the system will look for a remote named `origin`.

In case you get the error reported below, you will have to either rename or add a remote. [Need help?](https://git-scm.com/book/en/Git-Basics-Working-with-Remotes)

```text
CMake Warning at /path/to/Cjyx/CMake/FindGit.cmake:144 (message):
No remote origin set for git repository: /path/to/MyExtension
Call Stack (most recent call first):
/path/to/Cjyx/CMake/CjyxMacroExtractRepositoryInfo.cmake:99 (GIT_WC_INFO)
/path/to/Cjyx/CMake/CjyxExtensionCPack.cmake:55 (CjyxMacroExtractRepositoryInfo)
CMakeLists.txt:25 (include)
```

### Why ExtensionWizard failed to describe extension: "script does not set 'EXTENSION_HOMEPAGE'"?

The issue is that the your extension has a "non standard" layout and the wizard has no way of extracting the expected information.

Similar issue has been discussed and reported for the SPHARM-PDM or UKF extension.

First half of the solution would be to move the metadata from `Common.cmake` to `CMakeLists.txt` as it is done in [here](https://github.com/NIRALUser/SPHARM-PDM/commit/15badbabd930573a9e251d1fd525313ea000d028). Then, you could make sure there is a project() statement by following what is suggested [here](https://mantisarchive.slicer.org/view.php?id=3737#c12081).

If you prefer not to re-organize your extension, you could still contribute extension description file by creating it manually.

### Is project() statement allowed in extension CMakeLists.txt?

Following [Cjyx r22038](https://github.com/Slicer/Slicer/commit/93cc799c97ca0528fc4cf25a1e49791bd3742677), the project statement is required.

### Is call to "if(NOT Cjyx_SOURCE_DIR)" required to protect "find_package(Cjyx)" in extension CMakeLists.txt?

Following [Cjyx r22063](https://github.com/Slicer/Slicer/commit/d1d0699aeaff85d7500269c7aaa78afdf8d3aa4a), protecting call to `find_package(Slicer)` with `if(NOT Slicer_SOURCE_DIR)` is no longer needed and should be removed to keep code simpler and easier to maintain.

Before:

```text
cmake_minimum_required(VERSION 2.8.9)

if(NOT Cjyx_SOURCE_DIR)
  find_package(Cjyx COMPONENTS ConfigurePrerequisites)
endif()

if(NOT Cjyx_SOURCE_DIR)
  set(EXTENSION_NAME EmptyExtensionTemplate)
  set(EXTENSION_HOMEPAGE "https://www.slicer.org/wiki/Documentation/Nightly/Extensions/EmptyExtensionTemplate")  set(EXTENSION_CATEGORY "Examples")
  set(EXTENSION_CONTRIBUTORS "Jean-Christophe Fillion-Robin (Kitware)")
  set(EXTENSION_DESCRIPTION "This is an example of extension bundling N module(s)")
  set(EXTENSION_ICONURL "http://viewvc.slicer.org/viewvc.cgi/Slicer4/trunk/Extensions/Testing/EmptyExtensionTemplate/EmptyExtensionTemplate.png?revision=21746&view=co")
  set(EXTENSION_SCREENSHOTURLS "https://www.slicer.org/w/img_auth.php/4/42/Slicer-r19441-EmptyExtensionTemplate-screenshot.png")
endif()

if(NOT Cjyx_SOURCE_DIR)
  find_package(Cjyx REQUIRED)
  include(${Cjyx_USE_FILE})
endif()

add_subdirectory(ModuleA)

if(NOT Cjyx_SOURCE_DIR)
  include(${Cjyx_EXTENSION_CPACK})
endif()
```

After:

```text
cmake_minimum_required(VERSION 2.8.9)

find_package(Cjyx COMPONENTS ConfigurePrerequisites)

project(EmptyExtensionTemplate)

set(EXTENSION_HOMEPAGE "https://www.slicer.org/wiki/Documentation/Nightly/Extensions/EmptyExtensionTemplate")set(EXTENSION_CATEGORY "Examples")
set(EXTENSION_CONTRIBUTORS "Jean-Christophe Fillion-Robin (Kitware)")
set(EXTENSION_DESCRIPTION "This is an example of empty extension")
set(EXTENSION_ICONURL "http://viewvc.slicer.org/viewvc.cgi/Slicer4/trunk/Extensions/Testing/EmptyExtensionTemplate/EmptyExtensionTemplate.png?revision=21746&view=co")
set(EXTENSION_SCREENSHOTURLS "https://www.slicer.org/w/img_auth.php/4/42/Slicer-r19441-EmptyExtensionTemplate-screenshot.png")

find_package(Cjyx REQUIRED)
include(${Cjyx_USE_FILE})

add_subdirectory(ModuleA)

include(${Cjyx_EXTENSION_CPACK})
```

### Why is the --contribute option is not available with the ExtensionWizard?

Wizard contribute option is available only (1) if Cjyx is built with OpenSSL support or (2) directly from the nightly.

To build Cjyx with SSL support, you need to build (or download) Qt with SSL support and configure Cjyx with `-DCjyx_USE_PYTHONQT_WITH_OPENSSL:BOOL=ON`

### How to package third party libraries?

Extensions integrating third party libraries should follow the [SuperBuild extension template](https://github.com/Slicer/Slicer/tree/master/Utilities/Templates/Extensions/SuperBuild).

Each third party libraries will be configured and built using a dedicated `External_MyLib.cmake` file, the install location of binaries and libraries should be set to `Cjyx_INSTALL_BIN_DIR` and `Cjyx_INSTALL_LIB_DIR`.

Also, starting with [Cjyx r25959](http://viewvc.slicer.org/viewvc.cgi/Slicer4?view=revision&revision=25959), extension can package python modules and packages using `PYTHON_SITE_PACKAGES_SUBDIR` CMake variable to specify the install destination.

These relative paths are the one that the extensions manager will consider when generating the launcher and application settings for a given extension.

### Can I use C++14/17/20 language features?

We try to balance between compatibility and using new features. As a result, currently C++14 features are allowed, but usage of C++17/20 language features are discouraged in extensions (relying on C++17/20 features may lead to build errors on some build configurations).

If your extension can be compiled as a standalone project where you would like to use newer feature, you could rely on CMake detecting compile features. See [cmake-compile-features](https://cmake.org/cmake/help/v3.5/manual/cmake-compile-features.7.html) for more details.

See the labs topic on [upgrading compiler infrastructure](https://www.slicer.org/wiki/Documentation/Labs/UpgradingCompilerInfrastructure) for additional information/status.

### How do I publish a paper about my extension?

Consider publishing a paper describing your extension. [This page](https://www.software.ac.uk/resources/guides/which-journals-should-i-publish-my-software) contains a list of journals that publish papers about software. [Citing 3D Slicer](../user_guide/about.md#how-to-cite) in all papers that use 3D Slicer is greatly appreciated.

### How to force Cjyx to download extensions corresponding to a different Cjyx revision?

Since extensions available from the Extensions Manager are associated with a particular Cjyx revision, for development versions, typically no extension builds will appear in the Extensions Manager. For testing purposes, the current revision can be overridden in the Python interactor in Cjyx:

```python
>>> extensionManagerModel = cjyx.app.extensionsManagerModel()
>>> extensionManagerModel.cjyxRevision = "25742"
```

On other approach is to re-configure and build Cjyx setting the `Cjyx_FORCED_WC_REVISION` option.

### How to address ITK test driver caught an ITK exception "Could not create IO object for reading file"?

If the following exception is reported when trying to run tests associated with a CLI modules:

```text
 ITK test driver caught an ITK exception:

 itk::ImageFileReaderException (0x1bd8430)
 Location: "unknown"
 File: /path/to/Cjyx-SuperBuild/ITK/Modules/IO/ImageBase/include/itkImageFileReader.hxx
 Line: 139
 Description:  Could not create IO object for reading file /path/to/image.nrrd
   Tried to create one of the following:
     MRMLIDImageIO
   You probably failed to set a file suffix, or
     set the suffix to an unsupported type.
```

It most likely means that the test driver is not linking against `ITKFactoryRegistration` library and/or registrering the ITK factories. To address this, the test driver should be updated:
- link against `${CjyxExecutionModel_EXTRA_EXECUTABLE_TARGET_LIBRARIES}`
- include `itkFactoryRegistration.h`
- call `itk::itkFactoryRegistration();` in its main function.

For more details, read [What is the ITKFactoryRegistration library?](https://www.slicer.org/wiki/Documentation/Nightly/Developers/FAQ#What_is_the_ITKFactoryRegistration_library_.3F).

# Application settings

## Editing application settings

The application settings dialog allows users to customize application behavior.

After starting Cjyx, it can be accessed clicking in menu: `Edit` / `Application Settings`.

### General

Application startup script can be used to launch any custom Python code when Cjyx application is started.

### Modules

#### Skip loading

Select which [type of modules](https://www.slicer.org/wiki/Documentation/Nightly/Developers/Modules) to not load at startup. It is also possible to start Slicer by temporarily disabling those modules (not saved in settings) by passing the arguments in the command line.

For example, this command will start Cjyx without any CLI loaded:

    Cjyx.exe --disable-cli-modules

#### Show hidden modules

Some modules don't have a user interface, they are hidden from the module's list. For debugging purpose, it is possible to force their display

#### Temporary directory

Directory where modules can store their temporary outputs if needed.

#### Additional module paths

List of directories scanned at startup to load additional modules. Any CLI, Loadable or scripted modules located in these paths will be loaded. Extensions are listed in the list, to remove an extension, use the [Extensions Manager](extensions_manager.md) instead.

It is also possible to start Cjyx by temporarily adding module paths (not saved in settings) by passing the arguments in the command line.

For example this command will start Cjyx trying to load CLIs found in the specified directory:

    Cjyx.exe --additional-module-paths C:\path\to\lib\Cjyx-X.Y\cli-modules

#### Modules

List of modules loaded, ignored or failed to load in Cjyx. An unchecked checkbox indicates that module should not be loaded (ignored) next time Cjyx starts. A text color code is used to describe the state of each module:

-  Black: module successfully loaded in Cjyx
-  Gray: module not loaded because it has been ignored (unchecked)
-  Red: module failed to load. There are multiple reasons why a module can fail to load.

Look at startup [log outputs](https://www.slicer.org/wiki/Documentation/Nightly/SlicerApplication/ErrorLog) to have more information.
If a module is not loaded in Cjyx (ignored or failed), all dependent modules won't be loaded. You can verify the dependencies of a module in the tooltip of the module.

You can filter the list of modules by untoggling in the advanced (>>) panel the "To Load", "To Ignore", "Loaded", "Ignored" and "Failed" buttons.

#### Home

Module that is shown when Cjyx starts up.

#### Favorites

List of modules that appear in the Favorites toolbar:

![](https://github.com/Slicer/Slicer/releases/download/docs-resources/settings_favorites_toolbar.png)

To add a module, drag&drop it from the *Modules* list above. Then use the advanced panel (>>) to reorganize/delete the modules within the toolbar.

### Appearance

#### Style

The overall theme of Cjyx is controlled by the selected Style:
- Cjyx (default): it sets the style based on theme settings set by the operating system.
  For example, on Windows if [dark mode](https://blogs.windows.com/windowsexperience/2016/08/08/windows-10-tip-personalize-your-pc-by-enabling-the-dark-theme/)
  is turned on for apps, then the `Dark Cjyx` style will be used upon launching Cjyx. Currently, automatic detection of dark mode is not available on Linux,
  therefore use needs to manually select `Dark Cjyx` style for a dark color scheme.
- Light Cjyx: application window background is bright, regardless of operating system settings.
- Dark Cjyx: application window background is dark, regardless of operating system settings.

## Information for Advanced Users

### Settings file location

Settings are stored in *.ini files. If the settings file is found in application home directory (within organization name or domain subfolder) then that .ini file is used. This can be used for creating a portable application that contains all software and settings in a relocatable folder. Relative paths in settings files are resolved using the application home directory, and therefore are portable along with the application.

If .ini file is not found in the the application home directory then it is searched in user profile:

-  Windows: `%USERPROFILE%\AppData\Roaming\NA-MIC\` (typically `C:\Users\<your_user_name>\AppData\Roaming\NA-MIC\`)
-  Linux: `~/.config/NA-MIC/`
-  Mac: `~/.config/www.na-mic.org/`

Deleting the *.ini files restores all the settings to default.

There are two types of settings: `user specific settings` and `user and revision specific settings`.

#### User specific settings

This file is named `Cjyx.ini` and it stores settings applying to *all versions* of Cjyx installed by the *current user*.

To display the exact location of this settings file, open a terminal and type:

    ./Cjyx --settings-path

On Windows:

    Cjyx.exe --settings-path | more

or enter the following in the Python interactor:

    cjyx.app.cjyxUserSettingsFilePath

#### User and revision specific settings

This file is named like `Cjyx-<REVISION>.ini` and it stores settings applying to a *specific revision* of Cjyx installed by the *current user*.

To display the exact location of this settings file, enter the following in the Python interactor:

    cjyx.app.cjyxRevisionUserSettingsFilePath

### Application startup file

Each time Cjyx starts, it will look up for a startup script file named <code>.cjyxrc.py</code>. Content of this file is executed automatically at each startup of Cjyx.

The file is searched at multiple location and the first one that is found is used. Searched locations:
- Application home folder (`cjyx.app.cjyxHome`)
- Path defined in `CJYXRC` environment variable
- User profile folder (`~/.cjyxrc.py`)

You can find the path to the startup script in Cjyx by opening in the menu: Edit / Application Settings. ''Application startup script'' path is shown in the ''General'' section (or running `getCjyxRCFileName()` command in Cjyx Python console).

### Runtime environment variables

The following environment variables can be set before the application is started to fine-tune its behavior:
- `PYTHONNOUSERSITE`: if it is set to `1` then import of user site packages is disabled. For example, this will prevent Cjyx to reuse packages downloaded/built by Anaconda.
- `QT_SCALE_FACTOR`: see [Qt documentation](https://doc.qt.io/qt-5/highdpi.html). For example, font size can be reduced by running `set QT_SCALE_FACTOR=0.5` in the command console and then starting Slicer in that console.
- `QT_ENABLE_HIGHDPI_SCALING`: see [Qt documentation](https://doc.qt.io/qt-5/highdpi.html)
- `QT_SCALE_FACTOR_ROUNDING_POLICY`: see [Qt documentation](https://doc.qt.io/qt-5/highdpi.html)
- `QTWEBENGINE_REMOTE_DEBUGGING`: port number for Qt webengine remote debugger. Default value is `1337`.
- `CJYX_OPENGL_PROFILE`: Requested OpenGL profile. Valid values are `no` (no profile), `core` (core profile),
  and `compatibility` (compatibility profile). Default value is `compatibility` on Windows systems.
- `CJYX_BACKGROUND_THREAD_PRIORITY`: Set priority for background processing tasks. On Linux, it may affect the
  entire process priority. An integer value is expected, default = `20` on Linux and macOS, and `-1` on Windows.
- `CJYXRC`: Custom application startup file path. Contains a full path to a Python script. By default it is `~/.cjyxrc.py` (where ~ is the user profile a.k.a user home folder).
- `CJYX_EXTENSIONS_MANAGER_SERVER_URL`: URL of the extensions manager backend with the `/api` path. Default value is retrieved from the settings using the key `Extensions/ServerUrl`.
- `CJYX_EXTENSIONS_MANAGER_FRONTEND_SERVER_URL`: URL of the extension manager frontend displaying the web page. Default value is retrieved from the settings using the key `Extensions/FrontendServerUrl`.
- `CJYX_EXTENSIONS_MANAGER_SERVER_API`: Supported value is `Girder_v1`. Default value is hard-coded to `Girder_v1`.

### Qt built-in command-line options

Cjyx application accepts standard Qt command-line arguments that specify how Qt interacts with the windowing system.

Examples of options:
- `-qwindowgeometry geometry`, specifies window geometry for the main window using the X11-syntax. For example: `-qwindowgeometry 100x100+50+50`.
- `-display hostname:screen_number`, switches displays on X11 and overrides the `DISPLAY` environment variable.
- `-platform windows:dpiawareness=[0|1|2]`, sets the [DPI awareness](https://doc.qt.io/qt-5/highdpi.html#microsoft-windows) on Windows.
- `-widgetcount`, prints debug message at the end about number of widgets left undestroyed and maximum number of widgets existed at the same time.
- `-reverse`, sets the application's layout direction to `Qt::RightToLeft`.

To learn about the supported options:
- https://doc.qt.io/qt-5/qapplication.html#QApplication
- https://doc.qt.io/qt-5/qguiapplication.html#supported-command-line-options

:::{note}

Since the Cjyx launcher is itself a Qt application and the Qt built-in command-line options are expected to **only** be passed to
the launched application `CjyxApp-real` and not the Cjyx launcher, the list of arguments to filter is specified in the [Main.cpp](https://github.com/commontk/AppLauncher/blob/master/Main.cpp#L35) found in the `commontk/AppLauncher` project.

:::

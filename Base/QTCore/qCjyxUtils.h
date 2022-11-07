/*==============================================================================

  Program: 3D Cjyx

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#ifndef __qCjyxUtils_h
#define __qCjyxUtils_h

#include <QFile>
#include <QString>

#include "qCjyxBaseQTCoreExport.h"

class qCjyxAbstractCoreModule;

class Q_CJYX_BASE_QTCORE_EXPORT qCjyxUtils
{

public:
  typedef qCjyxUtils Self;

  /// Return true if the string name ends with one of these executable extension:
  /// ".bat", ".com", ".sh", ".csh", ".tcsh", ".pl", ".py", ".tcl", ".m", ".exe"
  /// \note The comparison is case insensitive
  static bool isExecutableName(const QString& name);

  /// Returns \a true if the \a filePath matches the CLI executable file name requirements
  static bool isCLIExecutable(const QString& filePath);

  /// Returns \a true if the \a filePath matches scripted CLI requirements
  static bool isCLIScriptedExecutable(const QString& filePath);

  /// Returns \a true if the \a filePath matches the CLI loadable module file name requirements.
  /// \note Associated \a fileName is expected to match the following
  /// regular expression: "(lib)?.+Lib\\.(dll|DLL|so|dylib)"
  static bool isCLILoadableModule(const QString& filePath);

  /// Return \a true if \a filePath matches the loadable module file name requirements.
  /// \note Associated \a fileName is expected to match the following
  /// regular expression: "(lib)?qCjyx.+Module\\.(so, dll, dylib)"
  static bool isLoadableModule(const QString& filePath);

  /// Return \a true is the module is for testing purposes.
  /// These modules are for testing and for troubleshooting and normally should not be displayed
  /// to end users.
  static bool isTestingModule(qCjyxAbstractCoreModule* module);

  /// Look for target file in build intermediate directory.
  /// On windows, the intermediate directory includes: . Debug RelWithDebInfo Release MinSizeRel
  /// And it return the first matched directory
  /// On the other plateform, this function just return the directory passed as a first argument
  static QString searchTargetInIntDir(const QString& directory, const QString& target);

  /// This function returns an empty string on all plateform expected windows
  /// where it returns ".exe"
  static QString executableExtension();

  /// This function returns ".dll. on windows, ".so" on linux and ".dylib" on Mac
  //static QString libraryExtension();

  /// Extract module name given a library name
  /// For example:
  ///  on linux, libThresholdLib.so -> Threshold
  ///  on mac, libThresholdLib.{dylib, bundle, so} -> Threshold
  ///  on windows, ThresholdLib.dll -> Threshold
  static QString extractModuleNameFromLibraryName(const QString& libraryName);

  /// Extract module name givew a \a className
  /// For example:
  ///  qCjyxThresholdModule -> Threshold
  static QString extractModuleNameFromClassName(const QString& className);

  /// Return \a true if the plugin identified with its \a filePath is loaded from an install tree.
  /// \warning Since internally the function looks for the existence of CMakeCache.txt, it will
  /// return an incorrect result if the plugin is installed in the build tree of
  /// an other project.
  static bool isPluginInstalled(const QString& filePath, const QString& applicationHomeDir);

  /// Return \a true if the plugin identified with its \a filePath is a built-in Cjyx module.
  static bool isPluginBuiltIn(const QString& filePath, const QString& applicationHomeDir, const QString& applicationRevision);

  /// Return the path without the intermediate directory or return \a path if there is no
  /// expected "IntDir".
  /// \a subDirWithoutIntDir corresponds to N last compononent of the path excluding
  /// the "IntDir".
  ///
  /// <table border="1">
  ///   <tr>
  ///     <td><b>Path</b></td>
  ///     <td><b>subDirWithoutIntDir</b></td>
  ///     <td><b>Return</b></td>
  ///     <td><b>IntDir</b></td>
  ///   </tr>
  ///   <tr>
  ///     <td>/path/to/lib/module/Foo</td>
  ///     <td>lib/module</td>
  ///     <td>/path/to/lib/module</td>
  ///     <td>Foo</td>
  ///   </tr>
  ///   <tr>
  ///     <td>/path/to/lib/module/Release</td>
  ///     <td>lib/module</td>
  ///     <td>/path/to/lib/module</td>
  ///     <td>Release</td>
  ///   </tr>
  ///   <tr>
  ///     <td>/path/to/lib/module/Release/</td>
  ///     <td>lib/module</td>
  ///     <td>/path/to/lib/module</td>
  ///     <td>Release</td>
  ///   </tr>
  ///   <tr>
  ///     <td>/path/to/lib/module/Release</td>
  ///     <td>module</td>
  ///     <td>/path/to/lib/module</td>
  ///     <td>Release</td>
  ///   </tr>
  ///   <tr>
  ///     <td>/path/to/lib/module/Release</td>
  ///     <td><i>(Empty string)</i></td>
  ///     <td>/path/to/lib/module/Release</td>
  ///     <td><i>(Empty string)</i></td>
  ///   </tr>
  ///   <tr>
  ///     <td>/path/to/lib/module/Release/foo.txt</td>
  ///     <td>lib/module</td>
  ///     <td>/path/to/lib/module/Release/foo.txt</td>
  ///     <td><i>(Empty string)</i></td>
  ///   </tr>
  ///   <tr>
  ///     <td>/path/to/lib/module/Release/foo.txt</td>
  ///     <td>lib/module/Release</td>
  ///     <td>/path/to/lib/module/Release</td>
  ///     <td>foo.txt</td>
  ///   </tr>
  ///   <tr>
  ///     <td>/path/to/lib/module/Release</td>
  ///     <td>/path/to/lib/module</td>
  ///     <td>/path/to/lib/module</td>
  ///     <td>Release</td>
  ///   </tr>
  ///   <tr>
  ///     <td>/path/to/bin</td>
  ///     <td>lib/module</td>
  ///     <td>/path/to/bin</td>
  ///     <td><i>(Empty string)</i></td>
  ///   </tr>
  ///   <tr>
  ///     <td>/path/to/Foo.app/Contents/MacOSX</td>
  ///     <td>bin</td>
  ///     <td>/path/to/Foo.app/Contents/MacOSX</td>
  ///     <td><i>(Empty string)</i></td>
  ///   </tr>
  /// </table>
  static QString pathWithoutIntDir(const QString& path, const QString& subDirWithoutIntDir);
  static QString pathWithoutIntDir(const QString& path, const QString& subDirWithoutIntDir, QString& intDir);

  /// Return \a true if the \a inputPath ends with \a path
  static bool pathEndsWith(const QString& inputPath, const QString& path);


  /// Set permissions of \a path. If \a path is a directory, permissions will be set to all its descendant.
  /// While recursively traversing the tree structure, if \a path or one of its descendant
  /// is a file, \a filePermissions will be set otherwise \a directoryPermissions will be set.
  /// \note If for some reason, permissions on a file or directory failed to be set, the
  /// traversing will be aborted and the permission already set won't be reverted.
  /// \return \c true on success, \c false otherwise.
  /// \sa QFile::setPermissions()
  static bool setPermissionsRecursively(const QString &path,
                                        QFile::Permissions directoryPermissions,
                                        QFile::Permissions filePermissions);

  /// \brief Return an updated \a text where Cjyx wiki URL version is replaced
  /// with the provided one. It is now deprecated as the Cjyx wiki is no longer
  /// recommended for storing documentation. Instead Cjyx core documentation
  /// is hosted on ReadTheDocs (and versioning can be implemented using replaceDocumentationUrlVersion)
  /// and extension documentation is expected to be hosted in each extension's repository
  /// (and modules in extensions can create versioned links for their documentation using
  /// qCjyxCoreApplication::documentationVersion()).
  ///
  /// Any URL containing `Documentation/<VERSION>/` will be updated.
  ///
  /// More specifically, in an URL with http or https protocol that contains
  /// "Documentation" path element, the following path element is replaced by
  /// the provided \a version. If there are multiple
  /// "Documentation" path elements then only the first one is considered.
  ///
  /// \note The "path element" term is defined in this rfc: https://tools.ietf.org/html/rfc3986
  /// "A path consists of a sequence of path segments separated by a slash
  /// ("/") character."
  ///
  /// Examples:
  /// \snippet qCjyxUtilsTest1.cxx replaceWikiUrlVersion example1
  /// \snippet qCjyxUtilsTest1.cxx replaceWikiUrlVersion example2
  /// \snippet qCjyxUtilsTest1.cxx replaceWikiUrlVersion example3
  /// \snippet qCjyxUtilsTest1.cxx replaceWikiUrlVersion example4
  static QString replaceWikiUrlVersion(const QString& text, const QString& version);

  /// \brief Return an updated \a text where Cjyx documentation URL version is replaced
  /// with the provided one.
  ///
  /// Any URL that contains `/<VERSION>/` and with URL host name that contains the provided hostname
  /// string will be updated.
  ///
  /// Following readthedocs versioning conventions, `<VERSION>` can be
  /// `<major>.<minor>`, `v<major>.<minor>`, `latest`, or `stable`.
  ///
  /// Only the first occurrence of version is replaced (to keep file or directory names that have
  /// a name that matches accepted version patterns unchanged).
  /// For example, in `https://slicer.readthedocs.io/en/latest/user_guide/schema/1.0/something.html`
  /// the `1.0` folder name will not be changed (only `latest` will be set to the actual version).
  ///
  /// Limitations:
  /// - language code is not updated (the language specified in the original URL will be used)
  /// - extensions that have versioned documentation maintained in their own repository have to
  ///   generate their versioned links (for example, using qCjyxCoreApplication::documentationVersion()).
  static QString replaceDocumentationUrlVersion(const QString& text, const QString& hostname, const QString& version);

private:
  /// Not implemented
  qCjyxUtils() = default;
  virtual ~qCjyxUtils() = default;

};

#endif
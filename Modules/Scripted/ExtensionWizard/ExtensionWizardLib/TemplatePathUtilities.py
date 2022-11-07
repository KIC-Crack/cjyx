import os

import cjyx

_userTemplatePathKey = "ExtensionWizard/TemplatePaths"


# -----------------------------------------------------------------------------
def userTemplatePathKey(category=None):
    if category is None:
        return _userTemplatePathKey
    else:
        return f"{_userTemplatePathKey}/{category}"


# -----------------------------------------------------------------------------
def builtinTemplatePath():
    # Look for templates in source directory first
    path = cjyx.util.sourceDir()

    if path is not None:
        path = os.path.join(path, "Utilities", "Templates")

        if os.path.exists(path):
            return path

    # Look for installed templates
    path = os.path.join(cjyx.app.cjyxHome, cjyx.app.cjyxSharePath,
                        "Wizard", "Templates")

    if os.path.exists(path):
        return path

    # No templates found
    return None

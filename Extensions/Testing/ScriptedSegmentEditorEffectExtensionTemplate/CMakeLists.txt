cmake_minimum_required(VERSION 3.16.3...3.19.7 FATAL_ERROR)

project(ScriptedSegmentEditorEffectExtensionTemplate)

#-----------------------------------------------------------------------------
# Extension meta-information
set(EXTENSION_HOMEPAGE "https://www.slicer.org/wiki/Documentation/Nightly/Extensions/ScriptedSegmentEditorEffectExtensionTemplate")
set(EXTENSION_CATEGORY "Examples")
set(EXTENSION_CONTRIBUTORS "John Doe (AnyWare Corp.)")
set(EXTENSION_DESCRIPTION "This is an example of a simple extension")
set(EXTENSION_ICONURL "https://www.example.com/Slicer/Extensions/ScriptedSegmentEditorEffectExtensionTemplate.png")
set(EXTENSION_SCREENSHOTURLS "https://www.example.com/Slicer/Extensions/ScriptedSegmentEditorEffectExtensionTemplate/Screenshots/1.png")
set(EXTENSION_DEPENDS "NA") # Specified as a list or "NA" if no dependencies

#-----------------------------------------------------------------------------
# Extension dependencies
find_package(Cjyx REQUIRED)
include(${Cjyx_USE_FILE})

#-----------------------------------------------------------------------------
# Extension modules
add_subdirectory(ScriptedSegmentEditorEffectModuleTemplate)
## NEXT_MODULE

#-----------------------------------------------------------------------------
include(${Cjyx_EXTENSION_GENERATE_CONFIG})
include(${Cjyx_EXTENSION_CPACK})

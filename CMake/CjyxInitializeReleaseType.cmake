
# Default release type to use of none was specified
if(NOT DEFINED Cjyx_DEFAULT_RELEASE_TYPE)
  set(Cjyx_DEFAULT_RELEASE_TYPE "Experimental")
endif()

# Cjyx Release type
set(Cjyx_RELEASE_TYPE "${Cjyx_DEFAULT_RELEASE_TYPE}" CACHE STRING "Type of Cjyx release.")
mark_as_advanced(Cjyx_RELEASE_TYPE)

# Set the possible values for cmake-gui
set_property(CACHE Cjyx_RELEASE_TYPE PROPERTY STRINGS
  "Experimental"
  "Preview"
  "Stable"
  )

mark_as_superbuild(Cjyx_RELEASE_TYPE)

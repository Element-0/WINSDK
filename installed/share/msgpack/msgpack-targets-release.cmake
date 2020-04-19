#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "msgpackc" for configuration "Release"
set_property(TARGET msgpackc APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(msgpackc PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/msgpackc.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/msgpackc.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS msgpackc )
list(APPEND _IMPORT_CHECK_FILES_FOR_msgpackc "${_IMPORT_PREFIX}/lib/msgpackc.lib" "${_IMPORT_PREFIX}/bin/msgpackc.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)

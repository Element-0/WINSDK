#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "LibLZMA::LibLZMA" for configuration "Release"
set_property(TARGET LibLZMA::LibLZMA APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LibLZMA::LibLZMA PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/lzma.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/lzma.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS LibLZMA::LibLZMA )
list(APPEND _IMPORT_CHECK_FILES_FOR_LibLZMA::LibLZMA "${_IMPORT_PREFIX}/lib/lzma.lib" "${_IMPORT_PREFIX}/bin/lzma.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)

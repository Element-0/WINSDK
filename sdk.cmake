function(init_user_prop prop)
  if(${prop})
    set(ENV{_${prop}} "${${prop}}")
  else()
    set(${prop} "$ENV{_${prop}}" PARENT_SCOPE)
  endif()
endfunction()

function(generate_winsdk_vfs_overlay winsdk_include_dir output_path)
  set(include_dirs)
  file(GLOB_RECURSE entries LIST_DIRECTORIES true "${winsdk_include_dir}")
  foreach(entry ${entries})
    if(IS_DIRECTORY "${entry}")
      list(APPEND include_dirs "${entry}")
    endif()
  endforeach()

  file(WRITE "${output_path}"  "version: 0\n")
  file(APPEND "${output_path}" "case-sensitive: false\n")
  file(APPEND "${output_path}" "roots:\n")

  foreach(dir ${include_dirs})
    file(GLOB headers RELATIVE "${dir}" "${dir}/*.h")
    if(NOT headers)
      continue()
    endif()

    file(APPEND "${output_path}" "  - name: \"${dir}\"\n")
    file(APPEND "${output_path}" "    type: directory\n")
    file(APPEND "${output_path}" "    contents:\n")

    foreach(header ${headers})
      file(APPEND "${output_path}" "      - name: \"${header}\"\n")
      file(APPEND "${output_path}" "        type: file\n")
      file(APPEND "${output_path}" "        external-contents: \"${dir}/${header}\"\n")
    endforeach()
  endforeach()
endfunction()

function(generate_lib_symlinks winsdk_lib_dir output_dir)
  execute_process(COMMAND "${CMAKE_COMMAND}" -E make_directory "${output_dir}")
  file(GLOB libraries RELATIVE "${winsdk_lib_dir}" "${winsdk_lib_dir}/*")
  foreach(library ${libraries})
    string(TOLOWER "${library}" symlink_name)
    get_filename_component(base "${symlink_name}" NAME_WE)
    get_filename_component(ex "${symlink_name}" EXT)
    string(TOUPPER "${base}" symlink_name_upper)
    execute_process(COMMAND "${CMAKE_COMMAND}"
                            -E create_symlink
                            "${winsdk_lib_dir}/${library}"
                            "${output_dir}/${symlink_name}")
    execute_process(COMMAND "${CMAKE_COMMAND}"
                            -E create_symlink
                            "${winsdk_lib_dir}/${library}"
                            "${output_dir}/${symlink_name_upper}${ex}")
  endforeach()
endfunction()

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_VERSION 10.0)
set(CMAKE_SYSTEM_PROCESSOR AMD64)
set(HOST_ARCH x86_64)
set(SDK_ROOT "${CMAKE_CURRENT_LIST_DIR}")

set(MSVC_INCLUDE "${SDK_ROOT}/msvc/include")
set(MSVC_LIB "${SDK_ROOT}/msvc/lib")
set(MSVC_BIN "${SDK_ROOT}/msvc/bin")
set(WINSDK_INCLUDE "${SDK_ROOT}/winsdk/include")
set(WINSDK_LIB "${SDK_ROOT}/winsdk/lib")
set(WINSDK_BIN "${SDK_ROOT}/winsdk/bin")

list(APPEND CMAKE_PROGRAM_PATH "${MSVC_BIN}" "${WINSDK_BIN}")

if(NOT EXISTS "${MSVC_INCLUDE}" OR NOT EXISTS "${MSVC_LIB}")
  message(SEND_ERROR "Failed to find msvc")
endif()

if(NOT EXISTS "${WINSDK_INCLUDE}" OR NOT EXISTS "${WINSDK_LIB}")
  message(SEND_ERROR "Failed to find winsdk")
endif()

if(NOT EXISTS "${WINSDK_INCLUDE}/Windows.h")
  message(SEND_ERROR "Cannot find Windows.h")
endif()
if(NOT EXISTS "${WINSDK_INCLUDE}/WINDOWS.H")
  set(case_sensitive_filesystem TRUE)
endif()

find_program(CLANG_CL_PATH NAMES clang-cl)
if(${CLANG_CL_PATH} STREQUAL "CLANG_CL_PATH-NOTFOUND")
  message(SEND_ERROR "Unable to find clang-cl")
endif()

find_program(LLD_LINK_PATH NAMES lld-link)
if(${LLD_LINK_PATH} STREQUAL "LLD_LINK_PATH-NOTFOUND")
  message(SEND_ERROR "Unable to find lld-link")
endif()

find_program(CLANG_C_PATH NAMES clang)
if(${CLANG_C_PATH} STREQUAL "CLANG_C_PATH-NOTFOUND")
  message(SEND_ERROR "Unable to find clang")
endif()

find_program(CLANG_CXX_PATH NAMES clang++)
if(${CLANG_CXX_PATH} STREQUAL "CLANG_CXX_PATH-NOTFOUND")
  message(SEND_ERROR "Unable to find clang++")
endif()

set(CMAKE_C_COMPILER "${CLANG_CL_PATH}" CACHE FILEPATH "")
set(CMAKE_CXX_COMPILER "${CLANG_CL_PATH}" CACHE FILEPATH "")
set(CMAKE_LINKER "${LLD_LINK_PATH}" CACHE FILEPATH "")

list(APPEND _CTF_NATIVE_DEFAULT "-DCMAKE_ASM_COMPILER=${CLANG_C_PATH}")
list(APPEND _CTF_NATIVE_DEFAULT "-DCMAKE_C_COMPILER=${CLANG_C_PATH}")
list(APPEND _CTF_NATIVE_DEFAULT "-DCMAKE_CXX_COMPILER=${CLANG_CXX_PATH}")

set(CROSS_TOOLCHAIN_FLAGS_NATIVE "${_CTF_NATIVE_DEFAULT}" CACHE STRING "")

set(COMPILE_FLAGS
    -DWIN32
    -DWINVER=0x0A00
    -D_CRT_SECURE_NO_WARNINGS
    --target=x86_64-windows-msvc
    -fms-compatibility-version=19.11
    -Wno-unused-command-line-argument
    -imsvc "${MSVC_INCLUDE}"
    -imsvc "${WINSDK_INCLUDE}")

if(case_sensitive_filesystem)
  # Ensure all sub-configures use the top-level VFS overlay instead of generating their own.
  init_user_prop(winsdk_vfs_overlay_path)
  if(NOT winsdk_vfs_overlay_path)
    set(winsdk_vfs_overlay_path "${CMAKE_BINARY_DIR}/winsdk_vfs_overlay.yaml")
    if(EXISTS "${winsdk_vfs_overlay_path}")
      message(STATUS "Generating winsdk vfs overlay (Skipped)")
    else()
      message(STATUS "Generating winsdk vfs overlay")
      generate_winsdk_vfs_overlay("${WINSDK_INCLUDE}" "${winsdk_vfs_overlay_path}")
    endif()
    init_user_prop(winsdk_vfs_overlay_path)
  endif()
  list(APPEND COMPILE_FLAGS
       -Xclang -ivfsoverlay -Xclang "${winsdk_vfs_overlay_path}")
endif()

string(REPLACE ";" " " COMPILE_FLAGS "${COMPILE_FLAGS}")

set(_CMAKE_C_FLAGS_INITIAL "${CMAKE_C_FLAGS}" CACHE STRING "")
set(CMAKE_C_FLAGS "${_CMAKE_C_FLAGS_INITIAL} ${COMPILE_FLAGS}" CACHE STRING "" FORCE)

set(_CMAKE_CXX_FLAGS_INITIAL "${CMAKE_CXX_FLAGS}" CACHE STRING "")
set(CMAKE_CXX_FLAGS "${_CMAKE_CXX_FLAGS_INITIAL} ${COMPILE_FLAGS}" CACHE STRING "" FORCE)

set(LINK_FLAGS /manifest:no -libpath:"${MSVC_LIB}" -libpath:"${WINSDK_LIB}")

if(case_sensitive_filesystem)
  init_user_prop(lib_symlinks_dir)
  if(NOT lib_symlinks_dir)
    set(lib_symlinks_dir "${CMAKE_BINARY_DIR}/winsdk_lib_symlinks")
    if(EXISTS "${lib_symlinks_dir}")
      message(STATUS "Generating winsdk lib symlinks (Skipped)")
    else()
      message(STATUS "Generating winsdk lib symlinks")
      generate_lib_symlinks("${WINSDK_LIB}" "${lib_symlinks_dir}")
      generate_lib_symlinks("${MSVC_LIB}" "${lib_symlinks_dir}")
    endif()
    init_user_prop(lib_symlinks_dir)
  endif()
  list(APPEND LINK_FLAGS -libpath:"${lib_symlinks_dir}")
endif()

string(REPLACE ";" " " LINK_FLAGS "${LINK_FLAGS}")

set(_CMAKE_EXE_LINKER_FLAGS_INITIAL "${CMAKE_EXE_LINKER_FLAGS}" CACHE STRING "")
set(CMAKE_EXE_LINKER_FLAGS "${_CMAKE_EXE_LINKER_FLAGS_INITIAL} ${LINK_FLAGS}" CACHE STRING "" FORCE)

set(_CMAKE_MODULE_LINKER_FLAGS_INITIAL "${CMAKE_MODULE_LINKER_FLAGS}" CACHE STRING "")
set(CMAKE_MODULE_LINKER_FLAGS "${_CMAKE_MODULE_LINKER_FLAGS_INITIAL} ${LINK_FLAGS}" CACHE STRING "" FORCE)

set(_CMAKE_SHARED_LINKER_FLAGS_INITIAL "${CMAKE_SHARED_LINKER_FLAGS}" CACHE STRING "")
set(CMAKE_SHARED_LINKER_FLAGS "${_CMAKE_SHARED_LINKER_FLAGS_INITIAL} ${LINK_FLAGS}" CACHE STRING "" FORCE)

set(CMAKE_C_STANDARD_LIBRARIES "" CACHE STRING "" FORCE)
set(CMAKE_CXX_STANDARD_LIBRARIES "" CACHE STRING "" FORCE)

set(VCPKG_INSTALLED_DIR "${SDK_ROOT}/installed")
list(APPEND CMAKE_PREFIX_PATH "${VCPKG_INSTALLED_DIR}")
list(APPEND CMAKE_LIBRARY_PATH "${VCPKG_INSTALLED_DIR}/lib/manual-link")
list(APPEND CMAKE_FIND_ROOT_PATH "${VCPKG_INSTALLED_DIR}")
list(APPEND CMAKE_PROGRAM_PATH "${VCPKG_INSTALLED_DIR}/tools")
file(GLOB _VCPKG_TOOLS_DIRS "${VCPKG_INSTALLED_DIR}/tools/*")
foreach(_VCPKG_TOOLS_DIR ${_VCPKG_TOOLS_DIRS})
  if(IS_DIRECTORY ${_VCPKG_TOOLS_DIR})
    list(APPEND CMAKE_PROGRAM_PATH ${_VCPKG_TOOLS_DIR})
  endif()
endforeach()

# macro(find_package name)
#   string(TOLOWER "${name}" _vcpkg_lowercase_name)
#   if(EXISTS "${VCPKG_INSTALLED_DIR}/share/${_vcpkg_lowercase_name}/vcpkg-cmake-wrapper.cmake")
#     set(ARGS "${ARGV}")
#     include(${VCPKG_INSTALLED_DIR}/share/${_vcpkg_lowercase_name}/vcpkg-cmake-wrapper.cmake)
#   elseif("${name}" STREQUAL "Boost" AND EXISTS "${VCPKG_INSTALLED_DIR}/include/boost")
#     set(Boost_USE_STATIC_LIBS OFF)
#     set(Boost_USE_MULTITHREADED ON)
#     unset(Boost_USE_STATIC_RUNTIME)
#     set(Boost_NO_BOOST_CMAKE ON)
#     unset(Boost_USE_STATIC_RUNTIME CACHE)
#     set(Boost_COMPILER "-vc140")
#     _find_package(${ARGV})
#   else()
#     _find_package(${ARGV})
#   endif()
# endmacro()

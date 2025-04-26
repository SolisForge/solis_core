#  =================================== SOLIS ===================================
#
#  This file contains macros high-level macros for registering targets and
#  exporting them.
#
#  Author    Meltwin
#  Date      05/04/25
#  Version   0.0.1
#  Copyright Meltwin - 2025
#             Distributed under the MIT Licence
#  =============================================================================
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(GenerateExportHeader)
include("${CMAKE_CURRENT_LIST_DIR}/arguments.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/package.cmake")

# =============================================================================
# Dependency call definition
# 
# Author: Meltwin
# Since: 0.0.1
# =============================================================================
macro(solis_depend dep_name)
  cmake_parse_arguments(ARG "" "" "" ${ARGN})
  find_package(${dep_name} ${ARG_UNPARSED_ARGUMENTS}) 
  solis_register(${dep_name} DEP)
endmacro()

# =============================================================================
# Interface target definition
# 
# Author: Meltwin
# Since: 0.0.1
# =============================================================================
macro(solis_interface lib_name)
  cmake_parse_arguments(ARG "" "NAMESPACE" "DEPENDS;INCLUDES" ${ARGN})

  # Generate library
  solis_namespace(OUT _ns TARGET ${lib_name} NAMESPACE ${ARG_NAMESPACE})
  message(STATUS "+ Processing interface \"${_ns}::${lib_name}\"")
  add_library(${lib_name} INTERFACE)
  set_target_properties(${lib_name} PROPERTIES OUTPUT_NAME "${_ns}_${lib_name}")
  add_library(${_ns}::${lib_name} ALIAS ${lib_name})
  set_target_properties(${lib_name} PROPERTIES EXPORT_NAME ${_ns}::${lib_name})
  solis_depends(${lib_name} DEPENDS ${ARG_DEPENDS})
  solis_include(${lib_name} INCLUDES ${ARG_INCLUDES} INTERFACE)
  solis_register(${lib_name} LIB)
endmacro()

# =============================================================================
# Library target definition
# 
# Author: Meltwin
# Since: 0.0.1
# =============================================================================
macro(solis_library lib_name)
  cmake_parse_arguments(ARG "" "NAMESPACE" "DIRECTORY;FILES;DEPENDS;INCLUDES;INCLUDES_RAW" ${ARGN})

  # Generate library
  solis_namespace(OUT _ns TARGET ${lib_name} NAMESPACE ${ARG_NAMESPACE})
  message(STATUS "+ Processing library \"${_ns}::${lib_name}\"")
  get_source_files(${lib_name}_SRC_FILES DIRECTORY ${ARG_DIRECTORY} FILES ${ARG_FILES})
  if (NOT "${${lib_name}_SRC_FILES}" STREQUAL "")
    add_library(${lib_name} SHARED ${${lib_name}_SRC_FILES})
    set_target_properties(${lib_name} PROPERTIES OUTPUT_NAME "${_ns}_${lib_name}")
    add_library(${_ns}::${lib_name} ALIAS ${lib_name})
    set_target_properties(${lib_name} PROPERTIES EXPORT_NAME ${_ns}::${lib_name})
    solis_depends(${lib_name} DEPENDS ${ARG_DEPENDS})
    solis_include(${lib_name} INCLUDES "include" ${ARG_INCLUDES} INCLUDES_RAW ${ARG_INCLUDES_RAW})
    solis_register(${lib_name} LIB)
  else()
    message(WARNING "No sources found for library ${PROJECT_NAME}/${lib_name}")
  endif()
endmacro()

# =============================================================================
# Executable target definition
# 
# Author: Meltwin
# Since: 0.0.1
# =============================================================================
macro(solis_program exe_name)
  cmake_parse_arguments(ARG "" "NAMESPACE" "DIRECTORY;FILES;DEPENDS;INCLUDES;INCLUDES_RAW" ${ARGN})

  # Generate executable
  solis_namespace(OUT _ns TARGET ${exe_name} NAMESPACE ${ARG_NAMESPACE})
  message(STATUS "+ Processing executable \"${_ns}::${exe_name}\"")
  get_source_files(${exe_name}_SRC_FILES DIRECTORY ${ARG_DIRECTORY} FILES ${ARG_FILES})
  if (NOT "${${exe_name}_SRC_FILES}" STREQUAL "")
    add_executable(${exe_name} ${${exe_name}_SRC_FILES})
    set_target_properties(${exe_name} PROPERTIES EXPORT_NAME ${_ns}::${exe_name})
    solis_depends(${exe_name} DEPENDS ${ARG_DEPENDS})
    solis_include(${exe_name} INCLUDES ${ARG_INCLUDES} INCLUDES_RAW ${ARG_INCLUDES_RAW})
    solis_register(${exe_name} EXE)
  else()
    message("No sources found for program ${PROJECT_NAME}/${exe_name}")
  endif()
endmacro()

# =============================================================================
# CMake target definition
# 
# Author: Meltwin
# Since: 0.0.1
# =============================================================================
macro(solis_cmake)
  cmake_parse_arguments(ARG "" "" "FILES;DIRECTORIES" ${ARGN})

  # Register from files
  message(STATUS "+ Processing cmake files")
  foreach(f ${ARG_FILES})
    solis_register(${f} CMAKE)
  endforeach()

  # Register from dir
  foreach(d ${ARG_DIRECTORIES})
    file(GLOB_RECURSE d_cmake LIST_DIRECTORIES FALSE "${PROJECT_SOURCE_DIR}/${d}/**.cmake")
    foreach(f ${d_cmake})
      solis_register(${f} CMAKE)
    endforeach()
  endforeach()
endmacro()

# =============================================================================
# Install and export all found targets.
# 
# Author: Meltwin
# Since: 0.0.1
# =============================================================================
macro(solis_package)
  cmake_parse_arguments(ARG "" "CONFIG_FILE" "" ${ARGN})

  # Export targets
  if (NOT "${${PROJECT_NAME}_MIXED_TARGETS}" STREQUAL "")
    solis_package_targets()
  endif()

  # Export cmake files
  if (NOT "${${PROJECT_NAME}_CMAKE_FILES}" STREQUAL "")
    solis_package_cmake()
  endif()

  solis_package_config(CONFIG_FILE ${ARG_CONFIG_FILE})
endmacro()
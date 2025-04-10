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
include("${solis_SOURCE_DIR}/cmake/arguments.cmake")
include("${solis_SOURCE_DIR}/cmake/package.cmake")

# Library target definition
# 
# Author: Meltwin
# Since: 0.0.1
macro(solis_library lib_name)
  cmake_parse_arguments(ARG "" "NAMESPACE" "DIRECTORY;FILES;DEPENDS;INCLUDES;INCLUDES_RAW" ${ARGN})

  # Generate library
  solis_namespace(OUT _ns TARGET ${lib_name} NAMESPACE ${ARG_NAMESPACE})
  message(STATUS "+ Processing library \"${_ns}::${lib_name}\"")
  get_source_files(${lib_name}_SRC_FILES DIRECTORY ${ARG_DIRECTORY} FILES ${ARG_FILES})
  if (NOT "${${lib_name}_SRC_FILES}" STREQUAL "")
    add_library(${lib_name} SHARED ${${lib_name}_SRC_FILES})
    set_target_properties(${lib_name} PROPERTIES EXPORT_NAME ${_ns}::${lib_name})
    solis_depends(${lib_name} DEPENDS ${ARG_DEPENDS})
    solis_include(${lib_name} INCLUDES ${ARG_INCLUDES} INCLUDES_RAW ${ARG_INCLUDES_RAW})
    solis_register(${lib_name} LIB)
  else()
    message(WARNING "No sources found for library ${PROJECT_NAME}/${lib_name}")
  endif()
endmacro()

# Executable target definition
# 
# Author: Meltwin
# Since: 0.0.1
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

# Install and export all found targets.
# 
# Author: Meltwin
# Since: 0.0.1
macro(solis_package)
  cmake_parse_arguments(ARG "" "" "" ${ARGN})
  set(BUILD_CMAKE_DIR "${PROJECT_BINARY_DIR}/share/${PROJECT_NAME}/cmake")
  set(INSTALL_CMAKE_DIR "${PROJECT_INSTALL_DIR}/share/${PROJECT_NAME}/cmake")

  if (NOT "${${PROJECT_NAME}_MIXED_TARGETS}" STREQUAL "")
    # Export targets
    solis_export_targets()
  endif()
  solis_export_config()
endmacro()
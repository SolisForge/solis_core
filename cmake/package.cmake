#  =================================== SOLIS ===================================
#
#  This file contains macros related to target definition, install and export.
#
#  Author    Meltwin
#  Date      05/04/25
#  Version   0.0.1
#  Copyright Meltwin - 2025
#             Distributed under the MIT Licence
#  =============================================================================

# =============================================================================
# Export the project targets in the packageTarget.cmake file
#
# Author: Meltwin
# Since 0.0.1
# =============================================================================
function(solis_package_targets)
  cmake_parse_arguments(_FUNC_ARG "" "" "" ${ARGN})
  message(STATUS "+ Exporting targets for package \"${PROJECT_NAME}\"")
  install(TARGETS ${${PROJECT_NAME}_LIB_TARGETS} ${${PROJECT_NAME}_EXE_TARGETS}
    EXPORT "${PROJECT_NAME}Targets"
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
  )
  # Export package's targets files
  export(EXPORT "${PROJECT_NAME}Targets"
    FILE "${PROJECT_BINARY_DIR}/share/${PROJECT_NAME}/cmake/${PROJECT_NAME}Targets.cmake"
  )
  install(EXPORT "${PROJECT_NAME}Targets"
      DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/share/${PROJECT_NAME}
  )
endfunction()

# =============================================================================
# Export the project CMake files
#
# Author: Meltwin
# Since 0.0.1
# =============================================================================
function(solis_package_cmake)
  cmake_parse_arguments(_FUNC_ARG "" "" "" ${ARGN})
  message(STATUS "+ Exporting CMake files for package \"${PROJECT_NAME}\"")
  install(
    FILES ${${PROJECT_NAME}_CMAKE_FILES}
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/share/${PROJECT_NAME}"
  )
endfunction()

# =============================================================================
# Export the project config in the projectConfig.cmake file
#
# Author: Meltwin
# Since 0.0.1
# =============================================================================
function(solis_package_config)
  cmake_parse_arguments(_FUNC_ARG "" "CONFIG_FILE" "" ${ARGN})
  message(STATUS "+ Exporting config for package \"${PROJECT_NAME}\"")

  # Exporting package config file
  set(BUILD_SHARE_DIR "${PROJECT_BINARY_DIR}/share/${PROJECT_NAME}")
  set(BUILD_CMAKE_DIR "${PROJECT_BINARY_DIR}/share/${PROJECT_NAME}/cmake")
  set(INSTALL_CMAKE_DIR "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/share/${PROJECT_NAME}/")

  # ---------------------------------------------------------------------------
  # Write file header
  # ---------------------------------------------------------------------------
  set(config_gen "${BUILD_CMAKE_DIR}/${PROJECT_NAME}Config.cmake.in")
  file(WRITE ${config_gen} "@PACKAGE_INIT@
  
# =============================== SOLIS AUTOGEN ===============================
")

  # ---------------------------------------------------------------------------
  # Add external dependencies
  # ---------------------------------------------------------------------------
  if (NOT "${${PROJECT_NAME}_EXTERNAL_DEPS}" STREQUAL "")
    message(VERBOSE "  Exporting external dependencies")
    file(APPEND ${config_gen} "
# ------------------ ${PROJECT_NAME} Deps -------------------
include(CMakeFindDependencyMacro)
")
    foreach(dep ${${PROJECT_NAME}_EXTERNAL_DEPS})
      file(APPEND ${config_gen} "find_dependency(${dep})\n")
    endforeach()
  endif()

  # ---------------------------------------------------------------------------
  # Export targets
  # ---------------------------------------------------------------------------
  if (NOT "${PROJECT_NAME}_MIXED_TARGETS" STREQUAL "") 
  message(VERBOSE "  Exporting project targets")
    file(APPEND ${config_gen} "
# ----------------- ${PROJECT_NAME} Targets -----------------
if (EXISTS \${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}Targets.cmake)
  include(\${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}Targets.cmake)
endif()
")
  endif()

  # ---------------------------------------------------------------------------
  # Add CMake imports
  # ---------------------------------------------------------------------------
  if (NOT "${${PROJECT_NAME}_CMAKE_FILES}" STREQUAL "")
    message(VERBOSE "  Exporting CMake extras")
    file(APPEND ${config_gen} "
# ------------------ ${PROJECT_NAME} CMake ------------------
macro(load_cmake _file)
  get_filename_component(_FILE_NAME \${_file} NAME)
  include(\${CMAKE_CURRENT_LIST_DIR}/\${_FILE_NAME})
endmacro()
")
    foreach(f ${${PROJECT_NAME}_CMAKE_FILES})
      file(APPEND ${config_gen} "load_cmake(${f})\n")
      configure_file(${f} ${BUILD_SHARE_DIR}/${f} COPYONLY)
    endforeach()
  endif()
  
  # ---------------------------------------------------------------------------
  # If project has its own custom config file
  # ---------------------------------------------------------------------------
  if (NOT "${_FUNC_ARG_CONFIG_FILE}" STREQUAL "" AND EXISTS ${PROJECT_SOURCE_DIR}/${_FUNC_ARG_CONFIG_FILE})
    message(VERBOSE "  Exporting custom config file")
    configure_package_config_file(
      ${PROJECT_SOURCE_DIR}/${_FUNC_ARG_CONFIG_FILE}
      ${BUILD_CMAKE_DIR}/${_FUNC_ARG_CONFIG_FILE}
      INSTALL_DESTINATION ${INSTALL_CMAKE_DIR}
    )
    install(
      FILES ${BUILD_CMAKE_DIR}/${_FUNC_ARG_CONFIG_FILE}
      DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/share/${PROJECT_NAME}"
    )
    file(APPEND ${config_gen} "
# ----------------- ${PROJECT_NAME} Custom ------------------
if (EXISTS \"\${CMAKE_CURRENT_LIST_DIR}/${_FUNC_ARG_CONFIG_FILE}\")
  include(\${CMAKE_CURRENT_LIST_DIR}/${_FUNC_ARG_CONFIG_FILE})
endif()
")
  endif()

  # ---------------------------------------------------------------------------
  # Exporting config file
  # ---------------------------------------------------------------------------
  message(VERBOSE "  Configuring config file")
  configure_package_config_file(
    ${config_gen}
    ${BUILD_CMAKE_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION ${INSTALL_CMAKE_DIR}/${PROJECT_NAME}Config.cmake
  )
  install(
    FILES ${BUILD_CMAKE_DIR}/${PROJECT_NAME}Config.cmake
    DESTINATION ${INSTALL_CMAKE_DIR}
  )
endfunction()
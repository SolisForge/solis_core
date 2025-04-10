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

# Export the project targets in the packageTarget.cmake file
#
# Author: Meltwin
# Since 0.0.1
function(solis_export_targets)
  cmake_parse_arguments(_FUNC_ARG "" "NAMESPACE" "" ${ARGN})
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
    FILE "${BUILD_CMAKE_DIR}/${PROJECT_NAME}Targets.cmake"
    NAMESPACE ${_FUNC_ARG_NAMESPACE}
  )
  install(EXPORT "${PROJECT_NAME}Targets"
      DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/share/${PROJECT_NAME}/
  )
endfunction()

# Export the project config in the projectConfig.cmake file
#
# Author: Meltwin
# Since 0.0.1
function(solis_export_config)
  cmake_parse_arguments(_FUNC_ARG "" "" "" ${ARGN})
  message(STATUS "+ Exporting config for package \"${PROJECT_NAME}\"")
  if(EXISTS "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake")
    message("Exporting config for ${PROJECT_NAME}")
    configure_package_config_file(
      cmake/${PROJECT_NAME}Config.cmake
      ${BUILD_CMAKE_DIR}/${PROJECT_NAME}Config.cmake
      INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/share/${PROJECT_NAME}/${PROJECT_NAME}Config.cmake
    )
    install(
      FILES ${BUILD_CMAKE_DIR}/${PROJECT_NAME}Config.cmake
      DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/share/${PROJECT_NAME}/
    )
  else()
    message("Could not find file ${CMAKE_CURRENT_LIST_DIR}/cmake/${PROJECT_NAME}Config.cmake")
  endif()
endfunction()
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

macro(export_config)
  message("Exporting config for ${PROJECT_NAME}")
  set(PROJECT_CONFIG_FILE "${PROJECT_BINARY_DIR}/share/${PROJECT_NAME}/cmake/${PROJECT_NAME}Config.cmake")
  set(INSTALL_CONFIG_DIR "${PROJECT_INSTALL_DIR}/share/${PROJECT_NAME}Config.cmake")
  configure_package_config_file(cmake/${PROJECT_NAME}Config.cmake
          ${PROJECT_CONFIG_FILE}
          INSTALL_DESTINATION ${INSTALL_CONFIG_DIR})
endmacro()

macro(testhello)
  message("Test hello")
endmacro()
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(GenerateExportHeader)

macro(solis_library lib_name)
  cmake_parse_arguments(ARG "" "DIRECTORY" "FILES;DEPENDS;INCLUDES" ${ARGN})

  # Get library files
  if (ARG_DIRECTORY)
    file(GLOB_RECURSE ${lib_name}_SRC_FILES LIST_DIRECTORIES false "${ARG_DIRECTORY}/**.cpp")
  endif()
  if (ARG_FILES)
    list(APPEND ${lib_name}_SRC_FILES ${ARG_FILES})
  endif()

  # Generate library
  if (NOT "${${lib_name}_SRC_FILES}" STREQUAL "")
    add_library(${lib_name} SHARED ${${lib_name}_SRC_FILES})
    if (ARG_DEPENDS)
      target_link_libraries(${lib_name} PUBLIC ${ARG_DEPENDS})
    endif()
    if (ARG_INCLUDES)
      target_include_directories(${lib_name} PUBLIC "include" ${ARG_INCLUDES})
    else()
      target_include_directories(${lib_name} PUBLIC "include")
    endif()
    list(APPEND ${PROJECT_NAME}_TARGETS ${lib_name})
  else()
    message("No sources found for library ${lib_name}")
  endif()
endmacro()

macro(solis_package)
  cmake_parse_arguments(ARG "" "NAMESPACE" "" ${ARGN})
  set(BUILD_CMAKE_DIR "${PROJECT_BINARY_DIR}/share/${PROJECT_NAME}/cmake")
  set(INSTALL_CMAKE_DIR "${PROJECT_INSTALL_DIR}/share/${PROJECT_NAME}/cmake")

  # Export targets
  if (NOT "${${PROJECT_NAME}_TARGETS}" STREQUAL "")
    message("Exporting targets for ${PROJECT_NAME}")
    install(TARGETS ${${PROJECT_NAME}_TARGETS} EXPORT "${PROJECT_NAME}Targets"
      LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
      ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
      RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
      INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
    export(EXPORT "${PROJECT_NAME}Targets"
      FILE "${BUILD_CMAKE_DIR}/${PROJECT_NAME}Targets.cmake"
      NAMESPACE ${ARG_NAMESPACE}
    )
  endif()

  # Exporting local libraries with namespace for include_directory integration
  foreach(lib ${${PROJECT_NAME}_TARGETS})
    add_library("${ARG_NAMESPACE}${lib}" ALIAS ${lib})
  endforeach()

  # Export package config
  if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/cmake/${PROJECT_NAME}Config.cmake")
    message("Exporting config for ${PROJECT_NAME}")
    configure_package_config_file(
      cmake/${PROJECT_NAME}Config.cmake
      ${BUILD_CMAKE_DIR}/${PROJECT_NAME}Config.cmake
      INSTALL_DESTINATION ${INSTALL_CMAKE_DIR}/${PROJECT_NAME}Config.cmake
    )
  else()
    message("Could not find file ${CMAKE_CURRENT_LIST_DIR}/cmake/${PROJECT_NAME}Config.cmake")
  endif()
endmacro()
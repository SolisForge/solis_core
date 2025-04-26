cmake_minimum_required(VERSION 3.25)
project(solis VERSION 0.0.1)

set(CXX_STANDARD 21)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -O3 -Wextra -Wpedantic -std=c++17)
endif()

include("cmake/solis.cmake")

solis_depend(ZLIB)
solis_depend(fmt)


# =============================================================================
# Export core files
# =============================================================================
solis_library(utils DIRECTORY "src/utils" DEPENDS ZLIB::ZLIB fmt::fmt)
solis_library(resources DIRECTORY "src/resources" )
solis_library(worlds DIRECTORY "src/worlds" DEPENDS utils resources)
solis_cmake(FILES
  cmake/arguments.cmake
  cmake/package.cmake
  cmake/utils.cmake
  cmake/solis.cmake
)


# =============================================================================
# Setup doctest
# =============================================================================
include(ExternalProject)
find_package(Git REQUIRED)

ExternalProject_Add(
    doctest
    TMP_DIR ${CMAKE_CURRENT_BINARY_DIR}/doctest/tmp
    STAMP_DIR ${CMAKE_CURRENT_BINARY_DIR}/doctest/stamp
    LOG_DIR ${CMAKE_CURRENT_BINARY_DIR}/doctest/logs
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/libs/doctest
    BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/doctest/doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    TIMEOUT 10
    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD OFF
)
solis_interface(cpp_test INCLUDES "libs/doctest/doctest")

solis_package()
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

solis_package(CONFIG_FILE "cmake/solisCustom.cmake")
cmake_minimum_required(VERSION 3.18)
project(solis CXX)

set(CXX_STANDARD 21)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -O3 -Wextra -Wpedantic -std=c++17)
endif()


include("cmake/extras.cmake")

# =============================================================================
# Export core files
# =============================================================================
file(GLOB_RECURSE lib_src LIST_DIRECTORIES false "src/**.cpp")
add_library(core SHARED ${lib_src})
target_include_directories(core
  PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/include>"
)

export_config()
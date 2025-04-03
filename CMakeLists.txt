cmake_minimum_required(VERSION 3.18)
project(solis VERSION 0.0.1)

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
macro(define_lib t)
  file(GLOB_RECURSE ${t}_src LIST_DIRECTORIES false "src/${t}/**.cpp")
  add_library(${t} SHARED ${${t}_src})
  target_include_directories(${t}
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/include>"
  )
endmacro()

define_lib(utils)
define_lib(resources)
define_lib(worlds)
target_link_libraries(worlds PRIVATE utils resources)

export_config()
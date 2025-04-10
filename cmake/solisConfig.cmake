@PACKAGE_INIT@

set(SOLIS_TARGETS "${CMAKE_CURRENT_LIST_DIR}/solisTargets.cmake")
if (EXISTS ${SOLIS_TARGETS})
  include(${SOLIS_TARGETS})
endif()

include(CMakeFindDependencyMacro)
find_dependency(ZLIB)
find_dependency(fmt)
message("Hello it's solis config file !")
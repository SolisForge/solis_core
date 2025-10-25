#  =================================== SOLIS ===================================
#
#  This file contains short-hand functions for quicker CMake files writing.
#
#  Author    Meltwin
#  Date      05/04/25
#  Version   0.0.1
#  Copyright Meltwin - 2025
#             Distributed under the MIT Licence
#  =============================================================================
include("${CMAKE_CURRENT_LIST_DIR}/utils.cmake")

# =============================================================================
# Get the target namespace.
# Arguments:
#   - TARGET: the target to get the namespace of.
#        If none are provided, return the project namespace.
#   - NAMESPACE: if provided, apply this namespace on the given target 
#        (or project if none)
#
# Author: Meltwin
# Since: 0.0.1
# =============================================================================
function(solis_namespace)
  cmake_parse_arguments(_FUNC_ARG "" "TARGET;NAMESPACE;OUT" "" ${ARGN})
  set(PRJ_NS "${PROJECT_NAME}_NAMESPACE")
  if ("${_FUNC_ARG_TARGET}" STREQUAL "") # Target is project
    if ("${_FUNC_ARG_NAMESPACE}" STREQUAL "" AND "${${PRJ_NS}}" STREQUAL "") # GET
      set_here_and_parent(${PRJ_NS} ${PROJECT_NAME})
    elseif(NOT "${_FUNC_ARG_NAMESPACE}" STREQUAL "") # SET
      set_here_and_parent(${PRJ_NS} ${_FUNC_ARG_NAMESPACE})
    endif()
    set(${_FUNC_ARG_OUT} ${${PRJ_NS}} PARENT_SCOPE)
  # -----------------------------------------------------------------
  else() # Target is a lib or exe
    # Variables
    set(TARGET_NS "${PROJECT_NAME}_${_FUNC_ARG_TARGET}_NAMESPACE")
    if ("${_FUNC_ARG_NAMESPACE}" STREQUAL "" AND "${${TARGET_NS}}" STREQUAL "") # GET
      solis_namespace(OUT pkg_ns)
      set_here_and_parent(${TARGET_NS} ${pkg_ns})
    elseif(NOT "${_FUNC_ARG_NAMESPACE}" STREQUAL "") # SET
      set_here_and_parent(${TARGET_NS} ${_FUNC_ARG_NAMESPACE})
    endif()
    set(${_FUNC_ARG_OUT} ${${TARGET_NS}} PARENT_SCOPE)
  endif()
endfunction()
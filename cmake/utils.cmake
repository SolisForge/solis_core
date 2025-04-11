#  =================================== SOLIS ===================================
#
#  This file contains util CMake methods.
#
#  Author    Meltwin
#  Date      05/04/25
#  Version   0.0.1
#  Copyright Meltwin - 2025
#             Distributed under the MIT Licence
#  =============================================================================

# =============================================================================
# Test if only one of the flag is ON.
#
# Author: Meltwin
# Since: 0.0.1
# =============================================================================
function(only_one_flag out)
  cmake_parse_arguments(_ARG "" "" "FLAGS" ${ARGN})
  set(FOUND_ONE FALSE)
  set(${out} TRUE PARENT_SCOPE)
  foreach(f ${_ARG_FLAGS})
    if (${f})
      if (${FOUND_ONE})
        set(${out} FALSE PARENT_SCOPE)
        break()
      else()
        set(FOUND_ONE TRUE)
      endif()
    endif()
  endforeach()
endfunction()

# =============================================================================
# Simple macro to wrap around the set command making it save at both local and
# parent scope.
#
# Author: Meltwin
# Since: 0.0.1
# =============================================================================
macro(set_here_and_parent _p _v)
  set(${_p} ${_v})
  set(${_p} ${_v} PARENT_SCOPE)
endmacro()

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
# Fetch the sources files from both directories and
# a file list.
#
# Author: Meltwin
# Since 0.0.1
# =============================================================================
function(get_source_files src_out)
  cmake_parse_arguments(_FUNC_ARG "" "" "DIRECTORY;FILES" ${ARGN})
  set(src_files "")
  if (_FUNC_ARG_DIRECTORY)
    foreach(dir ${_FUNC_ARG_DIRECTORY})
      message(VERBOSE "  Looking in ${PROJECT_SOURCE_DIR}/${dir}")
      file(GLOB_RECURSE src LIST_DIRECTORIES false "${PROJECT_SOURCE_DIR}/${dir}/**.cpp")
      list(APPEND src_files ${src})
    endforeach()
  endif()
  if (_FUNC_ARG_FILES)
    foreach(f ${_FUNC_ARG_FILES})
      message(VERBOSE "  Adding file ${f}")
    endforeach()
    list(APPEND src_files ${_FUNC_ARG_FILES})
  endif()
  set(${src_out} ${src_files} PARENT_SCOPE)
endfunction()


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

# =============================================================================
# Setup the include directories for the target.
#
# Author: Meltwin
# Since 0.0.1
# =============================================================================
function(solis_include _target) 
  cmake_parse_arguments(_FUNC_ARG "INTERFACE" "" "INCLUDES;INCLUDES_RAW" ${ARGN})
  if (_FUNC_ARG_INTERFACE)
    set(_FUNC_BUILD_ARGS "INTERFACE")
  else()
    set(_FUNC_BUILD_ARGS "PUBLIC")
  endif()

  # Make parameters for target_include_directories function
  set(include_dirs "")
  foreach(id ${_FUNC_ARG_INCLUDES})
    cmake_path(APPEND include_dirs "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${id}>$<INSTALL_INTERFACE:${id}>")
  endforeach()
  foreach(id ${_FUNC_ARG_INCLUDES_RAW})
    cmake_path(APPEND include_dirs "${CMAKE_CURRENT_SOURCE_DIR}/${id}")
  endforeach()

  # Include directory
  target_include_directories(${_target} ${_FUNC_BUILD_ARGS} ${include_dirs}) 
endfunction()

# =============================================================================
# Setup the library dependencies for the target.
#
# Author: Meltwin
# Since 0.0.1
# =============================================================================
function(solis_depends _target) 
  cmake_parse_arguments(_FUNC_ARG "" "" "DEPENDS" ${ARGN})
  if (_FUNC_ARG_DEPENDS)
    target_link_libraries(${_target} PUBLIC ${_FUNC_ARG_DEPENDS})
  endif()
endfunction()

# =============================================================================
# Register a target for autonomous packaging by the solis stack.
#
# Author: Meltwin
# Since: 0.0.1
# =============================================================================
function(solis_register _target)
  cmake_parse_arguments(_FUNC_ARG "EXE;LIB;CMAKE;DEP" "" "" ${ARGN})

  # Check for only one output flag
  only_one_flag(has_only_one FLAGS ${_FUNC_ARG_EXE} ${_FUNC_ARG_LIB} ${_FUNC_ARG_CMAKE} ${_FUNC_ARG_DEP})
  if (NOT ${has_only_one})
    message(SEND_ERROR " The solis_register() method can only have one flag !\nEXE=${_FUNC_ARG_EXE}, LIB=${_FUNC_ARG_LIB}, CMAKE=${_FUNC_ARG_CMAKE}, DEP=${_FUNC_ARG_DEP}")
    return()
  endif()

  # Register in the right list
  solis_namespace(OUT target_ns TARGET ${_target})
  set(target_name "\"${target_ns}::${_target}\"")
  if (${_FUNC_ARG_EXE})
    message(VERBOSE "  Registering executable ${target_name}")
    list(APPEND ${PROJECT_NAME}_EXE_TARGETS "${_target}")
    list(APPEND ${PROJECT_NAME}_MIXED_TARGETS "${_target}")
  elseif(${_FUNC_ARG_LIB})
    message(VERBOSE "  Registering library ${target_name}")
    list(APPEND ${PROJECT_NAME}_LIB_TARGETS "${_target}")
    list(APPEND ${PROJECT_NAME}_MIXED_TARGETS "${_target}")
  elseif(${_FUNC_ARG_CMAKE})
    message(VERBOSE "  Registering cmake files \"${_target}\"")
    list(APPEND ${PROJECT_NAME}_CMAKE_FILES "${_target}")
  elseif(${_FUNC_ARG_DEP})
    message(VERBOSE "  Registering external dependency \"${_target}\"")
    list(APPEND ${PROJECT_NAME}_EXTERNAL_DEPS "${_target}")
  else()
    message(WARNING "No flag supplied for target ${target_name}!")
    return()
  endif()

  # Add it in the mixed target list
  return(PROPAGATE 
    ${PROJECT_NAME}_EXE_TARGETS 
    ${PROJECT_NAME}_LIB_TARGETS 
    ${PROJECT_NAME}_CMAKE_FILES 
    ${PROJECT_NAME}_MIXED_TARGETS 
    ${PROJECT_NAME}_EXTERNAL_DEPS
  )
endfunction()
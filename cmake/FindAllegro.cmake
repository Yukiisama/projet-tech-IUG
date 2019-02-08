# - Try to find Allegro include dirs and libraries
# Usage of this module as follows:
#
#   find_package(Allegro 5.1 COMPONENTS main image primative color)
#
#   if(Allegro_FOUND)
#      include_directories(${Allegro_INCLUDE_DIRS})
#      add_executable(foo foo.cc)
#      target_link_libraries(foo ${Allegro_LIBRARIES})
#
#      add_executable(bar bar.cc)
#      target_link_libraries(bar ${Allegro_main_LIBRARIES})
#   endif()
#
# Variables defined by this module:
#
#   Allegro_FOUND                       System has Allegro, this means the
#                                       include dir was found, as well as all
#                                       the libraries specified in the
#                                       COMPONENTS list.
#
#   Allegro_INCLUDE_DIRS                Allegro include directories: not cached
#
#
#   Allegro_LIBRARIES                   Link to these to use the Allegro
#                                       libraries that you specified: not
#                                       cached
#
#   Allegro_LIBRARY_DIRS                The path to where the Allegro library
#                                       files are.
#
#   Allegro_VERSION                     The version number of the Allegro 
#                                       libraries that have been found
#
# For each component you specify in find_package(), the following (UPPER-CASE)
# variables are set.  You can use these variables if you would like to pick and
# choose components for your targets instead of just using Boost_LIBRARIES.
#
#   Allegro_${COMPONENT}_FOUND          True IF the Allegro library
#                                       "component" was found.
#
#   Allegro_${COMPONENT}_LIBRARY        Contains the libraries for the
#                                       specified Allegro "component"
#

#------------------------------------------------------------------------------
# Convenience msg macro
#------------------------------------------------------------------------------
macro(_Allegro_msg msg)
  if (NOT Allegro_FIND_QUIET)
    message(STATUS "Allegro: " ${msg})
  endif (NOT Allegro_FIND_QUIET)
endmacro(_Allegro_msg)

#------------------------------------------------------------------------------
# Setup some default variable values
#------------------------------------------------------------------------------
set(Allegro_FOUND FALSE)
set(Allegro_INCLUDE_DIRS )
set(Allegro_LIBRARIES )
set(Allegro_LIBRARY_DIRS )
set(Allegro_VERSION )

#------------------------------------------------------------------------------
# Appends relavent include_dirs, libraries, and library_dirs
#------------------------------------------------------------------------------
macro(_Allegro_add_component component)
  pkg_check_modules(
    Allegro_pc_${component} QUIET allegro_${component}-${Allegro_VERSION}
  )

  if (Allegro_pc_${component}_FOUND)
    list(APPEND Allegro_INCLUDE_DIRS ${Allegro_pc_${component}_INCLUDE_DIRS})
    list(APPEND Allegro_LIBRARIES ${Allegro_pc_${component}_LIBRARIES})
    list(APPEND Allegro_LIBRARY_DIRS ${Allegro_pc_${component}_LIBRARY_DIRS})
    set(Allegro_${component}_FOUND True)
    _Allegro_msg("Found allegro_${component}-${Allegro_VERSION}")
  else (Allegro_pc_${component}_FOUND)
    set(Allegro_${component}_FOUND False)
    _Allegro_msg("Could not find allegro_${component}-${Allegro_VERSION}")
    set(Allegro_FOUND False)
  endif (Allegro_pc_${component}_FOUND)
endmacro(_Allegro_add_component)

#------------------------------------------------------------------------------
# Appends relavent include_dirs, libraries, and library_dirs for the core lib
#------------------------------------------------------------------------------
macro(_find_Allegro)
  pkg_check_modules(Allegro_pc QUIET allegro-${Allegro_VERSION})

  if (Allegro_pc_FOUND)
    list(APPEND Allegro_INCLUDE_DIRS ${Allegro_pc_INCLUDE_DIRS})
    list(APPEND Allegro_LIBRARIES ${Allegro_pc_LIBRARIES})
    list(APPEND Allegro_LIBRARY_DIRS ${Allegro_pc_LIBRARY_DIRS})
    set(Allegro_FOUND True)
    _Allegro_msg("Found allegro-${Allegro_VERSION}")
  else (Allegro_pc_FOUND)
    set(Allegro_FOUND False)
    _Allegro_msg("Could not find allegro-${Allegro_VERSION}")
  endif (Allegro_pc_FOUND)
endmacro(_find_Allegro)

#------------------------------------------------------------------------------
# This module piggy backs on the pkg-config system provided by Allegro
#------------------------------------------------------------------------------
find_package(PkgConfig)
if (PKGCONFIG_FOUND)
  # Get the highest version of Allegro found in pkg-config
  execute_process(
    COMMAND ${PKG_CONFIG_EXECUTABLE} "--list-all"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    RESULT_VARIABLE pc_result
    OUTPUT_VARIABLE pc_out
  )
  string(REGEX MATCHALL "allegro-..." VERSION_STRS "${pc_out}")
  set(Allegro_VERSION 0)
  foreach (VERSION_STR_IT ${VERSION_STRS})
    string(REGEX MATCH "[0-9]" VERSION_STR_IT "${VERSION_STR_IT}")
    if (${VERSION_STR_IT} VERSION_GREATER ${Allegro_VERSION})
      set(Allegro_VERSION "${VERSION_STR_IT}")
    endif (${VERSION_STR_IT} VERSION_GREATER ${Allegro_VERSION})
  endforeach (VERSION_STR_IT ${VERSION_STRS})
  if (Allegro_VERSION VERSION_GREATER 0)
    _Allegro_msg("Using version ${Allegro_VERSION}")
    # No matter what find the core Allegro library
    _find_Allegro()
    if (Allegro_FIND_COMPONENTS)
      foreach (COMPONENT ${Allegro_FIND_COMPONENTS})
        STRING(TOLOWER ${COMPONENT} COMPONENT)
        _Allegro_add_component(${COMPONENT})
      endforeach (COMPONENT ${Allegro_FIND_COMPONENTS})
    endif (Allegro_FIND_COMPONENTS)
  else (Allegro_VERSION VERSION_GREATER 0)
    _Allegro_msg("Could not find any version of Allegro in pkg-config")
  endif (Allegro_VERSION VERSION_GREATER 0)
else (PKGCONFIG_FOUND)
  _Allegro_msg("Could not find pkg-config")
endif (PKGCONFIG_FOUND)

#------------------------------------------------------------------------------
# Make the lists unique
#------------------------------------------------------------------------------
list(REMOVE_DUPLICATES Allegro_INCLUDE_DIRS)
list(REMOVE_DUPLICATES Allegro_LIBRARIES)
if(Allegro_LIBRARY_DIRS)
  list(REMOVE_DUPLICATES Allegro_LIBRARY_DIRS)
endif()

#------------------------------------------------------------------------------
# If not found and REQUIRED error out
#------------------------------------------------------------------------------
if (Allegro_FIND_REQUIRED AND NOT Allegro_FOUND)
  message(FATAL_ERROR "Could not find Allegro")
endif (Allegro_FIND_REQUIRED AND NOT Allegro_FOUND)

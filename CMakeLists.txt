cmake_minimum_required (VERSION 2.8)

project(ProjetTechnoL3)
enable_language(CXX)

set(CMAKE_CXX_STANDARD 11)

# The project version number.
set(VERSION_MAJOR   0   CACHE STRING "Project major version number.")
set(VERSION_MINOR   0   CACHE STRING "Project minor version number.")
set(VERSION_PATCH   1   CACHE STRING "Project patch version number.")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake) 

if(MSVC)
	message("Please install Allegro 5.2 with Nuget (https://wiki.allegro.cc/index.php?title=Windows,_Visual_Studio_2015_and_Nuget_Allegro_5)"
		" and enable the addons Image, Color, Font and TrueType Font.")
else()
        find_package(Allegro 5.2 REQUIRED COMPONENTS image color font ttf primitives)
	if(NOT Allegro_FOUND)
		message(FATAL_ERROR "Allegro was not found, please install it first.")
	endif()

	include_directories(${Allegro_INCLUDE_DIRS})
	link_directories(${Allegro_LIBRARY_DIRS})
endif()

add_definitions(-DDATA_DIR="${PROJECT_SOURCE_DIR}/data/")

include(cmake/add_catch_tests.cmake) 

if(APPLE)
	link_directories("${PROJECT_SOURCE_DIR}/lib/_osx")
elseif(UNIX)
	link_directories("${PROJECT_SOURCE_DIR}/lib/_x11")
elseif(WIN32)
	link_directories("${PROJECT_SOURCE_DIR}/lib/_win32")
endif()

include_directories(include)

# target to generate libei
set(include_files
        include/ei_geometrymanager.h
        include/ei_eventmanager.h
        include/ei_application.h
        include/ei_main.h
        include/ei_widget.h
        include/ei_draw.h
        include/ei_event.h
        include/ei_types.h
        include/hw_interface.h
	)
add_custom_target(headers SOURCES ${include_files})

# target to generate libei
set(EI_SRC
        src/ei_draw.cpp
        )
add_library(ei ${EI_SRC})

# target to generate the test set
add_subdirectory(tests)

# target to generate API documentation with Doxygen
find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)." ${DOXYGEN_FOUND})

if(BUILD_DOCUMENTATION)
	if(NOT DOXYGEN_FOUND)
  	message(FATAL_ERROR "Doxygen is needed to build the documentation.")
  endif()

	set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
	set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

	configure_file(${doxyfile_in} ${doxyfile} @ONLY)

	add_custom_target(doc
		COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
	  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/docs
	  COMMENT "Generating API documentation with Doxygen"
	  VERBATIM)
endif()

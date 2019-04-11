
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

# minimal target
 add_executable(minimal minimal.cpp)
 target_link_libraries(minimal ei eibase ${Allegro_LIBRARIES} m)


# Targets disabled until the basic functionalities of the library are implemented

add_executable(frame frame.cpp)
target_link_libraries(frame ei eibase ${Allegro_LIBRARIES} m)

add_executable(button button.cpp)
target_link_libraries(button ei eibase ${Allegro_LIBRARIES} m)

add_executable(toplevel toplevel.cpp)
target_link_libraries(toplevel ei eibase ${Allegro_LIBRARIES} m)

add_executable(radiobutton radiobutton.cpp)
target_link_libraries(radiobutton ei eibase ${Allegro_LIBRARIES} m)

# Unit tests
add_executable(unit_tests unit_tests.cpp)
target_link_libraries(unit_tests ei eibase ${Allegro_LIBRARIES} m)


enable_testing()
ms_add_catch_tests(unit_tests)

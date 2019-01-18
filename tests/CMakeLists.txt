# minimal target
 add_executable(minimal minimal.cpp)
 target_link_libraries(minimal ei eibase ${Allegro_LIBRARIES} m)


# Targets disabled until the basic functionalities of the library are implemented

# add_executable(frame frame.cpp)
# target_link_libraries(frame ei eibase ${Allegro_LIBRARIES} m)

# add_executable(button button.cpp)
# target_link_libraries(button ei eibase ${Allegro_LIBRARIES} m)

# add_executable(toplevel toplevel.cpp)
# target_link_libraries(toplevel ei eibase ${Allegro_LIBRARIES} m)


# Unit tests
add_executable(unit_tests unit_tests.cpp)
target_link_libraries(unit_tests ei eibase ${Allegro_LIBRARIES} m)

enable_testing()
ms_add_catch_tests(unit_tests) 

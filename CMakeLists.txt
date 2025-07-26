cmake_minimum_required(VERSION 3.5)
project(PS5_Controller)

add_library(ps5_controller SHARED src/ps5_controller.cpp)
target_link_libraries(ps5_controller PUBLIC pthread)
target_include_directories(ps5_controller PUBLIC include)

add_executable(test_controller src/tests/test_controller.cpp)
target_link_libraries(test_controller PUBLIC ps5_controller)

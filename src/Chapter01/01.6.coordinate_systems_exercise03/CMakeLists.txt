cmake_minimum_required(VERSION 3.22.0)

project(01.6.coordinate_systems_exercise03)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(01.6.coordinate_systems_exercise03 "Chapter01")

target_link_libraries(01.6.coordinate_systems_exercise03 opengl32 glad glfw SharedUtils)

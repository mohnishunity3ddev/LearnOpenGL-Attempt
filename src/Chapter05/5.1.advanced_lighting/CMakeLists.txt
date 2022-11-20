cmake_minimum_required(VERSION 3.22.0)

project(5.1.advanced_lighting)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(5.1.advanced_lighting "Chapter05")

target_link_libraries(5.1.advanced_lighting opengl32 glad glfw SharedUtils)

cmake_minimum_required(VERSION 3.22.0)

project(4.10.1.instancing_instanced_arrays)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(4.10.1.instancing_instanced_arrays "Chapter04")

target_link_libraries(4.10.1.instancing_instanced_arrays opengl32 glad glfw SharedUtils)

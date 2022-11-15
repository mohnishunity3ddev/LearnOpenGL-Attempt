cmake_minimum_required(VERSION 3.22.0)

project(04.2.StencilTesting)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(04.2.StencilTesting "Chapter04")

target_link_libraries(04.2.StencilTesting opengl32 glad glfw SharedUtils)

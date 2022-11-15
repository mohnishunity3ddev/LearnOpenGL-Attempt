cmake_minimum_required(VERSION 3.22.0)

project(04.1.DepthTesting)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(04.1.DepthTesting "Chapter04")

target_link_libraries(04.1.DepthTesting opengl32 glad glfw SharedUtils)

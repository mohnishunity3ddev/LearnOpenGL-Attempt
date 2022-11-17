cmake_minimum_required(VERSION 3.22.0)

project(04.8.AdvancedGLSL)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(04.8.AdvancedGLSL "Chapter04")

target_link_libraries(04.8.AdvancedGLSL opengl32 glad glfw SharedUtils)

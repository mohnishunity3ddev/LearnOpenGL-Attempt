cmake_minimum_required(VERSION 3.22.0)

project(04.3.Blending)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(04.3.Blending "Chapter04")

target_link_libraries(04.3.Blending opengl32 glad glfw SharedUtils)

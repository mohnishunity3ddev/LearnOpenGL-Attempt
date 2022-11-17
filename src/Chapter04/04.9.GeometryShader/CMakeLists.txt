cmake_minimum_required(VERSION 3.22.0)

project(04.9.GeometryShader)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(04.9.GeometryShader "Chapter04")

target_link_libraries(04.9.GeometryShader opengl32 glad glfw SharedUtils)

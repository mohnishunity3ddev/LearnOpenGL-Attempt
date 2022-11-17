cmake_minimum_required(VERSION 3.22.0)

project(04.9.1.GeometryShader_BuildHouse)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(04.9.1.GeometryShader_BuildHouse "Chapter04")

target_link_libraries(04.9.1.GeometryShader_BuildHouse opengl32 glad glfw SharedUtils)

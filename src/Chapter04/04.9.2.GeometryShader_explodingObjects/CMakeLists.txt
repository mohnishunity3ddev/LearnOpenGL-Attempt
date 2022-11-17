cmake_minimum_required(VERSION 3.22.0)

project(04.9.2.GeometryShader_explodingObjects)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(04.9.2.GeometryShader_explodingObjects "Chapter04")

target_link_libraries(04.9.2.GeometryShader_explodingObjects opengl32 glad glfw SharedUtils assimp)

cmake_minimum_required(VERSION 3.22.0)

project(04.9.3.GeometryShader_VisualizeNormals)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(04.9.3.GeometryShader_VisualizeNormals "Chapter04")

target_link_libraries(04.9.3.GeometryShader_VisualizeNormals opengl32 glad glfw SharedUtils assimp)

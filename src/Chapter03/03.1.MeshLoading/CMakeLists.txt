cmake_minimum_required(VERSION 3.22.0)

project(03.1.MeshLoading)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(03.1.MeshLoading "Chapter03")

target_link_libraries(03.1.MeshLoading opengl32 glad glfw SharedUtils assimp)

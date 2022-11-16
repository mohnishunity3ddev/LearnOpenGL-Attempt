cmake_minimum_required(VERSION 3.22.0)

project(04.6.Cubemaps)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(04.6.Cubemaps "Chapter04")

target_link_libraries(04.6.Cubemaps opengl32 glad glfw SharedUtils assimp)

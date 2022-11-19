cmake_minimum_required(VERSION 3.22.0)

project(4.10.2.Instancing_AsteroidField)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(4.10.2.Instancing_AsteroidField "Chapter04")

target_link_libraries(4.10.2.Instancing_AsteroidField opengl32 glad glfw SharedUtils assimp)

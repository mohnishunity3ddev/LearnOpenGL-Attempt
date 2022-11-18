cmake_minimum_required(VERSION 3.22.0)

project(4.10.Instancing)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(4.10.Instancing "Chapter04")

target_link_libraries(4.10.Instancing opengl32 glad glfw SharedUtils)

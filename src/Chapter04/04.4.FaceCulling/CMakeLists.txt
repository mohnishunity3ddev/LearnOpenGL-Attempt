cmake_minimum_required(VERSION 3.22.0)

project(04.4.FaceCulling)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(04.4.FaceCulling "Chapter04")

target_link_libraries(04.4.FaceCulling opengl32 glad glfw SharedUtils)

cmake_minimum_required(VERSION 3.22.0)

project(02.3.Materials_exercise02)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(02.3.Materials_exercise02 "Chapter02")

target_link_libraries(02.3.Materials_exercise02 opengl32 glad glfw SharedUtils)

cmake_minimum_required(VERSION 3.22.0)

project(02.1.Colors)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(02.1.Colors "Chapter02")

target_link_libraries(02.1.Colors opengl32 glad glfw SharedUtils)

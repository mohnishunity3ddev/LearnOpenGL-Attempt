cmake_minimum_required(VERSION 3.22.0)

project(02.6.MultipleLights)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(02.6.MultipleLights "Chapter02")

target_link_libraries(02.6.MultipleLights opengl32 glad glfw SharedUtils)

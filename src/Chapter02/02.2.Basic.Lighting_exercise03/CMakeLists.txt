cmake_minimum_required(VERSION 3.22.0)

project(02.2.Basic.Lighting_exercise03)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(02.2.Basic.Lighting_exercise03 "Chapter02")

target_link_libraries(02.2.Basic.Lighting_exercise03 opengl32 glad glfw SharedUtils)

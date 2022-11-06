cmake_minimum_required(VERSION 3.22.0)

project(02.2.Basic.Lighting_exercise04_Gouraud)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(02.2.Basic.Lighting_exercise04_Gouraud "Chapter02")

target_link_libraries(02.2.Basic.Lighting_exercise04_Gouraud opengl32 glad glfw SharedUtils)

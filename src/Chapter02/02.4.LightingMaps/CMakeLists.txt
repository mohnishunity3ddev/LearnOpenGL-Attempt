cmake_minimum_required(VERSION 3.22.0)

project(02.4.LightingMaps)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(02.4.LightingMaps "Chapter02")

target_link_libraries(02.4.LightingMaps opengl32 glad glfw SharedUtils)

cmake_minimum_required(VERSION 3.22.0)

project(02.5.LightCasters)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(02.5.LightCasters "Chapter02")

target_link_libraries(02.5.LightCasters opengl32 glad glfw SharedUtils)

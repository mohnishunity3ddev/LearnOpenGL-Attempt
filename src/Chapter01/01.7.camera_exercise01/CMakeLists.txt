cmake_minimum_required(VERSION 3.22.0)

project(01.7.camera_exercise01)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(01.7.camera_exercise01 "Chapter01")

target_link_libraries(01.7.camera_exercise01 opengl32 glad glfw SharedUtils)

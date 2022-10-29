cmake_minimum_required(VERSION 3.22.0)

project(01.2.hello.triangle)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(01.2.hello.triangle "Chapter01")

target_link_libraries(${PROJECT_NAME} opengl32 glfw glad)
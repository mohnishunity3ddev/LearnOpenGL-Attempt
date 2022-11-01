cmake_minimum_required(VERSION 3.22.0)

project(01.4.textures_exercise04)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(01.4.textures_exercise04 "Chapter01")
target_link_libraries(${PROJECT_NAME} opengl32 glad glfw SharedUtils)
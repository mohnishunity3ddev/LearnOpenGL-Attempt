cmake_minimum_required(VERSION 3.22.0)

project(7.1.0.text_rendering)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(7.1.0.text_rendering "Chapter07")

target_link_libraries(7.1.0.text_rendering opengl32 glad glfw SharedUtils)

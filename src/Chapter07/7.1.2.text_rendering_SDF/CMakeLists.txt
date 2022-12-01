cmake_minimum_required(VERSION 3.22.0)

project(7.1.2.text_rendering_SDF)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(7.1.2.text_rendering_SDF "Chapter07")

target_link_libraries(7.1.2.text_rendering_SDF opengl32 glad glfw SharedUtils)

cmake_minimum_required(VERSION 3.22.0)

project(7.1.1.text_rendering_atlas)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(7.1.1.text_rendering_atlas "Chapter07")

target_link_libraries(7.1.1.text_rendering_atlas opengl32 glad glfw SharedUtils ${freetype})

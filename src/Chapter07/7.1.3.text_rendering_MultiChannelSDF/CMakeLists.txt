cmake_minimum_required(VERSION 3.22.0)

project(7.1.3.text_rendering_MultiChannelSDF)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(7.1.3.text_rendering_MultiChannelSDF "Chapter07")

target_link_libraries(7.1.3.text_rendering_MultiChannelSDF opengl32 glad glfw SharedUtils)

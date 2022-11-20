cmake_minimum_required(VERSION 3.22.0)

project(5.3.shadow_mapping)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(5.3.shadow_mapping "Chapter05")

target_link_libraries(5.3.shadow_mapping opengl32 glad glfw SharedUtils)

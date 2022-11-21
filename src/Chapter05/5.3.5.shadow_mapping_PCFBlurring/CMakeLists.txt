cmake_minimum_required(VERSION 3.22.0)

project(5.3.5.shadow_mapping_PCFBlurring)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(5.3.5.shadow_mapping_PCFBlurring "Chapter05")

target_link_libraries(5.3.5.shadow_mapping_PCFBlurring opengl32 glad glfw SharedUtils)

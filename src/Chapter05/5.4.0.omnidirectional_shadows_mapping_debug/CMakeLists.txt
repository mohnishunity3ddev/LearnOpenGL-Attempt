cmake_minimum_required(VERSION 3.22.0)

project(5.4.0.omnidirectional_shadows_mapping_debug)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(5.4.0.omnidirectional_shadows_mapping_debug "Chapter05")

target_link_libraries(5.4.0.omnidirectional_shadows_mapping_debug opengl32 glad glfw SharedUtils)

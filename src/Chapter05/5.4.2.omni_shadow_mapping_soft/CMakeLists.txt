cmake_minimum_required(VERSION 3.22.0)

project(5.4.2.omni_shadow_mapping_soft)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(5.4.2.omni_shadow_mapping_soft "Chapter05")

target_link_libraries(5.4.2.omni_shadow_mapping_soft opengl32 glad glfw SharedUtils)

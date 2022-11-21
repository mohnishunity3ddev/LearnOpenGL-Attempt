cmake_minimum_required(VERSION 3.22.0)

project(5.3.4.shadow_mapping_OverSampling)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(5.3.4.shadow_mapping_OverSampling "Chapter05")

target_link_libraries(5.3.4.shadow_mapping_OverSampling opengl32 glad glfw SharedUtils)

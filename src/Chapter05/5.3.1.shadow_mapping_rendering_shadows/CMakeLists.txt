cmake_minimum_required(VERSION 3.22.0)

project(5.3.1.shadow_mapping_rendering_shadows)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(5.3.1.shadow_mapping_rendering_shadows "Chapter05")

target_link_libraries(5.3.1.shadow_mapping_rendering_shadows opengl32 glad glfw SharedUtils)

cmake_minimum_required(VERSION 3.22.0)

project(5.3.3.shadow_mapping_peterPanning)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(5.3.3.shadow_mapping_peterPanning "Chapter05")

target_link_libraries(5.3.3.shadow_mapping_peterPanning opengl32 glad glfw SharedUtils)

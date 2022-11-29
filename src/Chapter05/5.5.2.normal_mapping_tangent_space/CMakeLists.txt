cmake_minimum_required(VERSION 3.22.0)

project(5.5.2.normal_mapping_tangent_space)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(5.5.2.normal_mapping_tangent_space "Chapter05")

target_link_libraries(5.5.2.normal_mapping_tangent_space opengl32 glad glfw SharedUtils)

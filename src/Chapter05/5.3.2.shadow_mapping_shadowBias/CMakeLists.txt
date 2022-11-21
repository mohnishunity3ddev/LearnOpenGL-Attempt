cmake_minimum_required(VERSION 3.22.0)

project(5.3.2.shadow_mapping_shadowBias)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(5.3.2.shadow_mapping_shadowBias "Chapter05")

target_link_libraries(5.3.2.shadow_mapping_shadowBias opengl32 glad glfw SharedUtils)

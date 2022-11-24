cmake_minimum_required(VERSION 3.22.0)

project(5.5.1.normal_mapping_complexMeshes)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(5.5.1.normal_mapping_complexMeshes "Chapter05")

target_link_libraries(5.5.1.normal_mapping_complexMeshes opengl32 glad glfw SharedUtils assimp)

cmake_minimum_required(VERSION 3.22.0)

project(5.2.gamma_correction)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(5.2.gamma_correction "Chapter05")

target_link_libraries(5.2.gamma_correction opengl32 glad glfw SharedUtils)

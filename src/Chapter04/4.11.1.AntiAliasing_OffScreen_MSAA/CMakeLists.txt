cmake_minimum_required(VERSION 3.22.0)

project(4.11.1.AntiAliasing_OffScreen_MSAA)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(4.11.1.AntiAliasing_OffScreen_MSAA "Chapter04")

target_link_libraries(4.11.1.AntiAliasing_OffScreen_MSAA opengl32 glad glfw SharedUtils)

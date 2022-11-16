cmake_minimum_required(VERSION 3.22.0)

project(04.5.Framebuffers_Exercise01)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(04.5.Framebuffers_Exercise01 "Chapter04")

target_link_libraries(04.5.Framebuffers_Exercise01 opengl32 glad glfw SharedUtils)

cmake_minimum_required(VERSION 3.22.0)

project(04.5.Framebuffers)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(04.5.Framebuffers "Chapter04")

target_link_libraries(04.5.Framebuffers opengl32 glad glfw SharedUtils)

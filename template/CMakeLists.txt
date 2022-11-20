cmake_minimum_required(VERSION 3.22.0)

project(opengl_template)

include(../cmake_macros/prac.cmake)

SETUP_APP(opengl_template "template")

target_link_libraries(opengl_template opengl32 glad glfw SharedUtils)

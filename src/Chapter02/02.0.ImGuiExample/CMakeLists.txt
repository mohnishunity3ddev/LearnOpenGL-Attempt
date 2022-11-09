cmake_minimum_required(VERSION 3.22.0)

project(02.0.ImGuiExample)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(02.0.ImGuiExample "Chapter02")

target_link_libraries(02.0.ImGuiExample opengl32 glad glfw SharedUtils)

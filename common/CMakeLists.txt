cmake_minimum_required(VERSION 3.22.0)

project(SharedUtils CXX C)

# generate project path in .h file inside commons folder.
configure_file(project_paths.h.in ${CMAKE_CURRENT_SOURCE_DIR}/project_paths.h @ONLY)

file(GLOB_RECURSE SRC_FILES     LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.c??)
file(GLOB_RECURSE HEADER_FILES  LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h)

add_library(SharedUtils ${SRC_FILES} ${HEADER_FILES})

set_property(TARGET SharedUtils PROPERTY CXX_STANDARD 20)
set_property(TARGET SharedUtils PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(SharedUtils PUBLIC opengl32 glad)
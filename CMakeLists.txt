cmake_minimum_required(VERSION 3.10)

project(Snake)

# Tells CMake to prefer using GLVND (OpenGL Vendor Neutral Dispatch) against older libGL
cmake_policy(SET CMP0072 NEW)

set(CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu/cmake/glfw3" ${CMAKE_PREFIX_PATH})

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(snake src/main.cpp src/glad.c src/Text.cpp)

set(FREETYPE_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/include/freetype")
set(FREETYPE_LIBRARY "${CMAKE_SOURCE_DIR}/libfreetype.so")

target_include_directories(snake PRIVATE "${CMAKE_SOURCE_DIR}/include" ${FREETYPE_INCLUDE_DIRS})

target_link_libraries(snake OpenGL::GL glfw ${FREETYPE_LIBRARY})

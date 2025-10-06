cmake_minimum_required(VERSION 4.0)
project(glfwtest2)

set(CMAKE_CXX_STANDARD 20)

add_executable(glfwtest2 src/main.cpp
        src/glad.c
        src/stb_image.cpp
        include/camera.h)

include_directories(include)
find_package(glfw3 REQUIRED)
target_link_libraries(glfwtest2 PRIVATE glfw)
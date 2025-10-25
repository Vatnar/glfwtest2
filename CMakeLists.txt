cmake_minimum_required(VERSION 4.0)
project(ShaderPreview LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_subdirectory(Vendor)
add_subdirectory(Logger)
add_subdirectory(ShaderPreview)
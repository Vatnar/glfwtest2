include(FetchContent)
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG e69e5f977d458f2650bb346dadf2ad30c5320281) # 10.2.1

FetchContent_MakeAvailable(fmt)
add_library(stb_image STATIC stb/src/stb_image.cpp)
target_include_directories(stb_image PUBLIC stb/include)

add_library(glad STATIC glad/src/glad.c)
target_include_directories(glad PUBLIC glad/include)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE glm)

add_library(khr INTERFACE)
target_include_directories(khr INTERFACE khr)

find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)

add_library(vendor INTERFACE)
target_link_libraries(vendor INTERFACE
        glad
        stb_image
        glm
        khr
        glfw
        assimp
)
add_executable(ShaderPreview
        src/main.cpp
        src/Mesh.cpp
        src/Model.cpp
        src/Shader.cpp
        src/SphereModel.cpp
        src/Camera.cpp
)

target_include_directories(ShaderPreview PRIVATE include)

target_link_libraries(ShaderPreview PRIVATE
        Logger
        vendor
        fmt::fmt
)
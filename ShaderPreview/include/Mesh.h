#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <string>
#include <vector>

#include "Types.h"


class Shader;
struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture
{
    uint        id;
    std::string type;
    std::string path;
};

class Mesh
{
public:
    // mesh data
    std::vector<Vertex>  vertices;
    std::vector<uint>    indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<uint> indices, std::vector<Texture> textures);
    void Draw(const Shader &shader) const;

private:
    uint VAO{}, VBO{}, EBO{};

    void setupMesh();
};

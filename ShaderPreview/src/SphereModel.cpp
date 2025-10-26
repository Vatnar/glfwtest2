#include "SphereModel.h"

#include "glad/glad.h"

#include <cmath>
#include <vector>
#include "fmt/chrono.h"
#include "glm/ext/scalar_constants.hpp"

#include "Shader.h"

SphereModel::SphereModel(const f32 radius, const usize sectorCount, const size_t stackCount)
{
    std::vector<f32>  vertices;
    std::vector<uint> indices;

    for (usize i = 0; i <= stackCount; ++i)
    {
        f32 stackAngle = glm::pi<f32>() / 2 - (f32) i * glm::pi<f32>() / (f32) stackCount;
        f32 xy         = radius * cosf(stackAngle);
        f32 z          = radius * sinf(stackAngle);

        for (usize j = 0; j <= sectorCount; ++j)
        {
            f32 sectorAngle = (f32) j * 2.0f * glm::pi<f32>() / (f32) sectorCount;
            f32 x           = xy * cosf(sectorAngle);
            f32 y           = xy * sinf(sectorAngle);
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }

    for (usize i = 0; i <= stackCount; ++i)
    {
        for (usize j = 0; j <= sectorCount; ++j)
        {
            uint first  = i * (sectorCount + 1) + j;
            uint second = first + sectorCount + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }
    indexCount = indices.size();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, (long) (vertices.size() * sizeof(f32)), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (long) (indices.size() * sizeof(unsigned int)), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), (void *) nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}
void SphereModel::Draw(const Shader &shader) const
{
    shader.use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (int) indexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

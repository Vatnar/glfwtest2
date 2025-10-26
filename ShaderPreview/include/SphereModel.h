#pragma once
#include "Types.h"


class Shader;
class SphereModel

{
public:
    explicit SphereModel(f32 radius = 1.0f, usize sectorCount = 16, size_t stackCount = 16);

    void Draw(const Shader &shader) const;

private:
    uint   VAO = 0, VBO = 0, EBO = 0;
    size_t indexCount = 0;
};

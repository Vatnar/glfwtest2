#pragma once
#include <glm/mat4x4.hpp>

#include "Logger.h"
#include "Types.h"

class Shader
{
public:
    uint ID;
    Shader(const char *vertexPath, const char *fragmentPath);

    void use() const;

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, f32 value) const;
    void setMat4(const std::string &name, const glm::mat4 &value) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec3(const std::string &name, f32 x, f32 y, f32 z) const;

private:
    static Logger logger;
};

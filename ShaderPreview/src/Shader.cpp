#include "glad/glad.h"

#include <fstream>
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    std::string   vertexCode;
    std::string   fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure &e)
    {
        logger.Error(fmt::format("File not successfully read at {} or {}", vertexPath, fragmentPath));
    }
    const char *vShaderCode{vertexCode.c_str()};
    const char *fShaderCode{fragmentCode.c_str()};

    // 2. compile shaders

    uint vertex{}, fragment{};
    int  success{};
    bool abortAtEnd   = false;
    char infoLog[512] = {}; // null terminate

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        abortAtEnd = true;
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        logger.Error(std::string("Vertex shader compilation failed\n") + infoLog);
    };

    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        abortAtEnd = true;
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        logger.Error(std::string("Fragment shader compilation failed\n") + infoLog);
    };

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        abortAtEnd = true;
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        logger.Error(std::string("Shader linking failed\n") + infoLog);
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (abortAtEnd)
    {
        logger.Error("Failed creating shader, aborting");
        std::abort();
    }
    logger.Info("Successfully compiled and linked shader");
}

void Shader::use() const { glUseProgram(ID); }
void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
}
void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, f32 value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setMat4(const std::string &name, const glm::mat4 &value) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}
void Shader::setVec3(const std::string &name, const f32 x, const f32 y, const f32 z) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(glm::vec3(x, y, z)));
}
Logger Shader::logger{"Shader"};

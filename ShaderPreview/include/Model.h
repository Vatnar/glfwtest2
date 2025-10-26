#pragma once
#include "Logger.h"
#include "assimp/scene.h"

#include "Mesh.h"

struct Texture;
class Shader;

unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

class Model
{
public:
    explicit Model(const char *path);
    void Draw(const Shader &shader) const;

private:
    std::vector<Mesh>    meshes;
    std::string          directory;
    std::vector<Texture> textures_loaded;

    void                 loadModel(const std::string &path);
    void                 processNode(const aiNode *node, const aiScene *scene);
    Mesh                 processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(const aiMaterial *mat, aiTextureType type, const std::string &typeName);
    static uint          TextureFromFile(const char *path, const std::string &directory);
    static Logger        logger;
};

#ifndef __JMODEL_H
#define __JMODEL_H

#include <vector>
#include <string>

#include <shader/shader.h>
#include "mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
    std::vector<MeshTexture> textures_loaded;

public:
    Model(const char* path, bool isPathRelative = true, bool flipImage = true);
    void Draw(const Shader &shader);

private:
    std::vector<Mesh> meshes;
    std::string directory;

    void loadModel(const std::string &path, bool isPathRelative);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<MeshTexture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif
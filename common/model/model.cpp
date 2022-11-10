#include "model.h"
#include <project_paths.h>

#include <glm/glm.hpp>
#include <helpers.h>

#include <iostream>

Model::Model(const char *path, bool isPathRelative) {
    loadModel(path, isPathRelative);
}

void Model::Draw(const Shader &shader) {
    for(unsigned int i = 0; i < meshes.size(); ++i) {
        meshes[i].Draw(shader);
    }
}

void Model::loadModel(const std::string &path, bool isPathRelative) {
    
    if(isPathRelative) {
        const_cast<std::string&>(path) = models_dir_path + path;
    }

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << "\n";
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    // Process all node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; ++i) {
        // Node's mMeshes array contains the index into the scene's mMeshes array and returns an aiMesh pointer.
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    // then do the same for all the node's children.
    for(unsigned int i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<VertexAttribute> vertices;
    std::vector<unsigned int> indices;
    std::vector<MeshTexture> textures;

    // Process vertex positions, normals and texture coordinates.
    // ---------------------------------------------------------
    for(unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        VertexAttribute vertex;


        // Vertex Positions
        glm::vec3 tempVec;
        tempVec.x = mesh->mVertices[i].x;
        tempVec.y = mesh->mVertices[i].y;
        tempVec.z = mesh->mVertices[i].z;
        vertex.position = tempVec;

        // Vertex Normals
        tempVec.x = mesh->mNormals[i].x;
        tempVec.y = mesh->mNormals[i].y;
        tempVec.z = mesh->mNormals[i].z;
        vertex.normal = tempVec;

        // Texture Coordinates
        /// Assimp allows every vertex to have 8 different texture coordinates per vertex. We only want the first set of tex coords.
        if(mesh->mTextureCoords[0]) {
            glm::vec2 tmp;
            tmp.x = mesh->mTextureCoords[0][i].x;
            tmp.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = tmp;
        } else {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }


        vertices.push_back(vertex);
    }

    // Process Indices.
    // ---------------------------------------------------------
    for(unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }


    // Process Materials.
    // ---------------------------------------------------------
    if(mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<MeshTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<MeshTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<MeshTexture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
    std::vector<MeshTexture> textures;

    for(unsigned int i = 0; i < mat->GetTextureCount(type); ++i) {
        aiString str;
        mat->GetTexture(type, i, &str);

        // Checking if the texture at the "str" path is already loaded once.
        bool skip = false;
        for(unsigned int j = 0; j < textures_loaded.size(); ++j) {
            if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }

        if(!skip) {
            // If this is a new texture which has not been loaded.
            MeshTexture texture;
            texture.id = TextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }

    return textures;
}
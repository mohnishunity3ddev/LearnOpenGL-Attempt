#ifndef __JMESH_H
#define __JMESH_H

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include <shader/shader.h>

struct VertexAttribute {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

struct MeshTexture {
    // id of the texture
    unsigned int id;
    // type of the shader. diffuse/specular
    std::string type;
    std::string path;
};

class Mesh {
public:
    // mesh
    std::vector<VertexAttribute> vertices;
    std::vector<unsigned int> indices;
    std::vector<MeshTexture> textures;

    Mesh(const std::vector<VertexAttribute> &vertices, const std::vector<unsigned int> &indices, const std::vector<MeshTexture> &textures);
    void Draw(const Shader& shader);
    unsigned int getVAO() { return VAO; }
private:
    // render data
    unsigned int VAO, VBO, EBO;
    void setupMesh();
};

#endif
#ifndef _PRIMITIVE_MESH
#define _PRIMITIVE_MESH

#include <glad/glad.h>
#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.inl>
#include <glm/gtc/type_ptr.hpp>

struct MeshInfo {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texcoords;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;
};

void setupVertexData(MeshInfo mesh, float* vertices) {

    for(unsigned int i = 0; i < mesh.vertices.size(); i += 3) {
        const glm::vec3& v0 = mesh.vertices[i];
        const glm::vec3& v1 = mesh.vertices[i + 1];
        const glm::vec3& v2 = mesh.vertices[i + 2];

        const glm::vec2& uv0 = mesh.texcoords[i];
        const glm::vec2& uv1 = mesh.texcoords[i + 1];
        const glm::vec2& uv2 = mesh.texcoords[i + 2];

        glm::vec3 deltaPos1 = v1 - v0;
        glm::vec3 deltaPos2 = v2 - v0;

        glm::vec2 deltaUV1 = uv1 - uv0;
        glm::vec2 deltaUV2 = uv2 - uv0;

        float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
        glm::vec3 tangent = (deltaPos1 * deltaUV2.y   - deltaPos2 * deltaUV1.y) * r;
        glm::vec3 bitangent = (deltaPos2 * deltaUV1.x   - deltaPos1 * deltaUV2.x) * r;

        mesh.tangents.push_back(tangent);
        mesh.tangents.push_back(tangent);
        mesh.tangents.push_back(tangent);

        mesh.bitangents.push_back(bitangent);
        mesh.bitangents.push_back(bitangent);
        mesh.bitangents.push_back(bitangent);
    }

    assert(mesh.vertices.size() == mesh.tangents.size());
    assert(mesh.vertices.size() == mesh.bitangents.size());

    for(unsigned int i = 0; i < mesh.vertices.size(); i++) {
            
        int index = i * 14;
        // vertices
        vertices[index + 0] = mesh.vertices[i].x;
        vertices[index + 1] = mesh.vertices[i].y;
        vertices[index + 2] = mesh.vertices[i].z;

        // normals
        vertices[index + 3] = mesh.normals[i].x;
        vertices[index + 4] = mesh.normals[i].y;
        vertices[index + 5] = mesh.normals[i].z;

        // texcoords
        vertices[index + 6] = mesh.texcoords[i].x;
        vertices[index + 7] = mesh.texcoords[i].y;

        // tangents
        vertices[index + 8]  = mesh.tangents[i].x;
        vertices[index + 9]  = mesh.tangents[i].y;
        vertices[index + 10] = mesh.tangents[i].z;

        // bitangents
        vertices[index + 11] = mesh.bitangents[i].x;
        vertices[index + 12] = mesh.bitangents[i].y;
        vertices[index + 13] = mesh.bitangents[i].z;
    }
}

void setupBufferObjects(unsigned int* VAO, unsigned int* VBO, const unsigned int size, const float* vertices) {
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);
    glBindVertexArray(*VAO);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); // vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1); // normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2); // texcoords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(3); // tangents
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(4); // bitangents
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

class PrimitiveMesh {
private:
    static PrimitiveMesh* instance_;

    MeshInfo cubeMesh;
    unsigned int cubeVAO = 0, cubeVBO = 0;

    MeshInfo quadMesh;
    unsigned int quadVAO = 0, quadVBO = 0;

    unsigned int planeVAO = 0, planeVBO = 0;
    unsigned int skyboxVAO = 0, skyboxVBO = 0;

public:

    static PrimitiveMesh* GetInstance() {
        if(instance_ == nullptr) {
            instance_ = new PrimitiveMesh();
        }
        return instance_;
    }

    void RenderCube() {
        if(instance_ == nullptr || instance_ != this) {
            std::cout << "Only single instance of cubemesh allowed.\n";
            return;
        }

        if(cubeVAO == 0) {
            cubeMesh.vertices = {
                // back face
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::vec3( 1.0f,  1.0f, -1.0f),
                glm::vec3( 1.0f, -1.0f, -1.0f),
                glm::vec3( 1.0f,  1.0f, -1.0f),
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::vec3(-1.0f,  1.0f, -1.0f),
                // front face
                glm::vec3(-1.0f, -1.0f,  1.0f),
                glm::vec3( 1.0f, -1.0f,  1.0f),
                glm::vec3( 1.0f,  1.0f,  1.0f),
                glm::vec3( 1.0f,  1.0f,  1.0f),
                glm::vec3(-1.0f,  1.0f,  1.0f),
                glm::vec3(-1.0f, -1.0f,  1.0f),
                // left face
                glm::vec3(-1.0f,  1.0f,  1.0f),
                glm::vec3(-1.0f,  1.0f, -1.0f),
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::vec3(-1.0f, -1.0f,  1.0f),
                glm::vec3(-1.0f,  1.0f,  1.0f),
                // right face
                glm::vec3( 1.0f,  1.0f,  1.0f),
                glm::vec3( 1.0f, -1.0f, -1.0f),
                glm::vec3( 1.0f,  1.0f, -1.0f),
                glm::vec3( 1.0f, -1.0f, -1.0f),
                glm::vec3( 1.0f,  1.0f,  1.0f),
                glm::vec3( 1.0f, -1.0f,  1.0f),
                // bottom face
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::vec3( 1.0f, -1.0f, -1.0f),
                glm::vec3( 1.0f, -1.0f,  1.0f),
                glm::vec3( 1.0f, -1.0f,  1.0f),
                glm::vec3(-1.0f, -1.0f,  1.0f),
                glm::vec3(-1.0f, -1.0f, -1.0f),
                // top face
                glm::vec3(-1.0f,  1.0f, -1.0f),
                glm::vec3( 1.0f,  1.0f , 1.0f),
                glm::vec3( 1.0f,  1.0f, -1.0f),
                glm::vec3( 1.0f,  1.0f,  1.0f),
                glm::vec3(-1.0f,  1.0f, -1.0f),
                glm::vec3(-1.0f,  1.0f,  1.0f),
            };  

            cubeMesh.texcoords = {
                glm::vec2(0.0f, 0.0f),
                glm::vec2(1.0f, 1.0f),
                glm::vec2(1.0f, 0.0f),
                glm::vec2(1.0f, 1.0f),
                glm::vec2(0.0f, 0.0f),
                glm::vec2(0.0f, 1.0f),

                glm::vec2(0.0f, 0.0f),
                glm::vec2(1.0f, 0.0f),
                glm::vec2(1.0f, 1.0f),
                glm::vec2(1.0f, 1.0f),
                glm::vec2(0.0f, 1.0f),
                glm::vec2(0.0f, 0.0f),

                glm::vec2(1.0f, 0.0f),
                glm::vec2(1.0f, 1.0f),
                glm::vec2(0.0f, 1.0f),
                glm::vec2(0.0f, 1.0f),
                glm::vec2(0.0f, 0.0f),
                glm::vec2(1.0f, 0.0f),

                glm::vec2(1.0f, 0.0f),
                glm::vec2(0.0f, 1.0f),
                glm::vec2(1.0f, 1.0f),
                glm::vec2(0.0f, 1.0f),
                glm::vec2(1.0f, 0.0f),
                glm::vec2(0.0f, 0.0f),

                glm::vec2(0.0f, 1.0f),
                glm::vec2(1.0f, 1.0f),
                glm::vec2(1.0f, 0.0f),
                glm::vec2(1.0f, 0.0f),
                glm::vec2(0.0f, 0.0f),
                glm::vec2(0.0f, 1.0f),

                glm::vec2(0.0f, 1.0f),
                glm::vec2(1.0f, 0.0f),
                glm::vec2(1.0f, 1.0f),
                glm::vec2(1.0f, 0.0f),
                glm::vec2(0.0f, 1.0f),
                glm::vec2(0.0f, 0.0f) 
            };      

            cubeMesh.normals = {
                glm::vec3( 0.0f,  0.0f, -1.0f),
                glm::vec3( 0.0f,  0.0f, -1.0f),
                glm::vec3( 0.0f,  0.0f, -1.0f),
                glm::vec3( 0.0f,  0.0f, -1.0f),
                glm::vec3( 0.0f,  0.0f, -1.0f),
                glm::vec3( 0.0f,  0.0f, -1.0f),

                glm::vec3( 0.0f,  0.0f,  1.0f),
                glm::vec3( 0.0f,  0.0f,  1.0f),
                glm::vec3( 0.0f,  0.0f,  1.0f),
                glm::vec3( 0.0f,  0.0f,  1.0f),
                glm::vec3( 0.0f,  0.0f,  1.0f),
                glm::vec3( 0.0f,  0.0f,  1.0f),

                glm::vec3(-1.0f,  0.0f,  0.0f),
                glm::vec3(-1.0f,  0.0f,  0.0f),
                glm::vec3(-1.0f,  0.0f,  0.0f),
                glm::vec3(-1.0f,  0.0f,  0.0f),
                glm::vec3(-1.0f,  0.0f,  0.0f),
                glm::vec3(-1.0f,  0.0f,  0.0f),

                glm::vec3( 1.0f,  0.0f,  0.0f),
                glm::vec3( 1.0f,  0.0f,  0.0f),
                glm::vec3( 1.0f,  0.0f,  0.0f),
                glm::vec3( 1.0f,  0.0f,  0.0f),
                glm::vec3( 1.0f,  0.0f,  0.0f),
                glm::vec3( 1.0f,  0.0f,  0.0f),

                glm::vec3( 0.0f, -1.0f,  0.0f),
                glm::vec3( 0.0f, -1.0f,  0.0f),
                glm::vec3( 0.0f, -1.0f,  0.0f),
                glm::vec3( 0.0f, -1.0f,  0.0f),
                glm::vec3( 0.0f, -1.0f,  0.0f),
                glm::vec3( 0.0f, -1.0f,  0.0f),

                glm::vec3( 0.0f,  1.0f,  0.0f),
                glm::vec3( 0.0f,  1.0f,  0.0f),
                glm::vec3( 0.0f,  1.0f,  0.0f),
                glm::vec3( 0.0f,  1.0f,  0.0f),
                glm::vec3( 0.0f,  1.0f,  0.0f),
                glm::vec3( 0.0f,  1.0f,  0.0f)
            };
        
            int numFloats = 14 * cubeMesh.vertices.size();
            int dataSize = numFloats * sizeof(float);
            float vertices[numFloats];

            setupVertexData(cubeMesh, vertices);
            
            setupBufferObjects(&cubeVAO, &cubeVBO, dataSize, vertices);
        }

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }

    void RenderQuad() {
        if(instance_ == nullptr || instance_ != this) {
            std::cout << "Only single instance of cubemesh allowed.\n";
            return;
        }

        if (quadVAO == 0) {
            quadMesh.vertices = {
                glm::vec3(-1.0f,  1.0f, 0.0f),
                glm::vec3(-1.0f, -1.0f, 0.0f),
                glm::vec3( 1.0f,  1.0f, 0.0f),
                glm::vec3( 1.0f, -1.0f, 0.0f),
                glm::vec3(-1.0f, -1.0f, 0.0f),
                glm::vec3( 1.0f,  1.0f, 0.0f)
            };

            quadMesh.normals = {
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(0.0f, 0.0f, 1.0f)
            };

            quadMesh.texcoords = {
                glm::vec2(0.0f, 1.0f),
                glm::vec2(0.0f, 0.0f),
                glm::vec2(1.0f, 1.0f),
                glm::vec2(1.0f, 0.0f),
                glm::vec2(0.0f, 0.0f),
                glm::vec2(1.0f, 1.0f)
            };

            unsigned int numFloats = 14 * quadMesh.vertices.size();
            unsigned int dataSize = numFloats * sizeof(float);
            float vertices[numFloats];

            setupVertexData(quadMesh, vertices);

            setupBufferObjects(&quadVAO, &quadVBO, dataSize, vertices);
        }
        glBindVertexArray(quadVAO);
        // TODO: Make this more optimized this can be done using GL_TRIANGLE_STRIP with 4 vertices instead of 6.
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    void RenderPlane() {
        if(instance_ == nullptr || instance_ != this) {
            std::cout << "Only single instance of cubemesh allowed.\n";
            return;
        }

        if (planeVAO == 0) {
            // set up vertex data (and buffer(s)) and configure vertex attributes
            // ------------------------------------------------------------------
            float planeVertices[] = {
                // positions            // normals         // texcoords
                10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
                -10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
                -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

                10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
                -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
                10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
            };

            // plane VAO
            glGenVertexArrays(1, &planeVAO);
            glGenBuffers(1, &planeVBO);
            glBindVertexArray(planeVAO);
            glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
            glBindVertexArray(0);
        }

        glBindVertexArray(planeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    void RenderSkybox() {
        if(instance_ == nullptr || instance_ != this) {
            std::cout << "Only single instance of cubemesh allowed.\n";
            return;
        }

        // Skybox VAO
        if(skyboxVAO == 0) {

            float skyboxVertices[] = {
                // positions          
                -1.0f,  1.0f, -1.0f,
                -1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,

                -1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,

                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,

                -1.0f, -1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,

                -1.0f,  1.0f, -1.0f,
                1.0f,  1.0f, -1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f, -1.0f,

                -1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                1.0f, -1.0f,  1.0f
            };

            glGenVertexArrays(1, &skyboxVAO);
            glGenBuffers(1, &skyboxVBO);
            glBindVertexArray(skyboxVAO);
            glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), 0);
            glEnableVertexAttribArray(0);
            glBindVertexArray(0);
        }
        glBindVertexArray(skyboxVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }
};

PrimitiveMesh* PrimitiveMesh::instance_ = nullptr;

#endif // _PRIMITIVE_MESH


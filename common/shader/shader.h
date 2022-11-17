#pragma once
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
    // The Shader Program ID.
    unsigned int ID;

    /**
     * @brief Constructor reads and builds the shader.
     * 
     * @param vertexPath The path of the vertex shader file.
     * @param fragmentPath The path of the fragment shader file.
     */
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);

    Shader() = delete;
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&&) = delete;
    Shader& operator=(Shader&&) = delete;
    ~Shader();

    // Use/Activate the Shader.
    void use() const;

    void setBool (const std::string &name, bool  value) const;
    void setInt  (const std::string &name, int   value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec3 (const std::string &name, const float *value) const;
    void setVec3 (const std::string &name, const float v1, const float v2, const float v3) const;
    void setVec3Array (const std::string &name, const int count, const float *value) const;
    void setMat4f(const std::string &name, int count, bool isTranspose, const float* value) const;
    void setMat4f(const std::string &name, const glm::mat4& value) const;

    // TODO: Remove this.
    void setTestColor(const std::string& name, float value) const;
};
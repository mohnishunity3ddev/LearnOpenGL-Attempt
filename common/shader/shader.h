#pragma once
#include <string>


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
    Shader(const char* vertexPath, const char* fragmentPath);

    Shader() = delete;
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&&) = delete;
    Shader& operator=(Shader&&) = delete;
    ~Shader();

    // Use/Activate the Shader.
    void use() const;

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;

    // TODO: Remove this.
    void setTestColor(const std::string& name, float value) const;
};
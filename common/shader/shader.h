#pragma once
#include <string>


class Shader {
public:
    // The Shader Program ID.
    unsigned int ID;

    // Constructor reads and builds the shader.
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    // Use/Activate the Shader.
    void use();

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;

    // TODO: Remove this.
    void setTestColor(const std::string& name, float value) const;
};
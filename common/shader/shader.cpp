#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {

    // 1. Retrieve the shader file contents.
    std::string vertexCode, fragmentCode;
    std::ifstream vShaderFile, fShaderFile;

    // ensure ifstream objects can throw exceptions
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        // Open files.
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        // read file's buffer contents into streams.
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // close file handlers.
        vShaderFile.close();
        fShaderFile.close();

        // convert stream to string.
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } 
    catch(std::ifstream::failure e) {
        std::cout << "Shader File Read Error: " << e.what() << "\n"; 
        exit(EXIT_FAILURE);
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. Compile, Link Shaders.
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success) {
        memset(infoLog, 0, 512);
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "Vertex Shader Compile Error: " << infoLog << " in vertex shader: " << vertexPath << "\n";
        return;
    }

    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success) {
        memset(infoLog, 0, 512);
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "Fragment Shader Compile Error: " << infoLog << " in fragment shader: " << fragmentPath << "\n";
        return;
    }

    // Shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success) {
        memset(infoLog, 0, 512);
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "Shader Program Link Error: " << infoLog << "\n";
        return;
    }

    // delete the shaders after attaching and linking them.
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {
    glDeleteProgram(ID);
}

void Shader::use() const {
    glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setTestColor(const std::string& name, float value) const {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), value, value, value, value);
}

void Shader::setMat4f(const std::string& name, int count, bool isTranspose, const float* value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), count, isTranspose, value);
}

void Shader::setVec3 (const std::string &name, const float *value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, value);
}

void Shader::setVec3Array (const std::string &name, const int count, const float *value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), count, value);
}




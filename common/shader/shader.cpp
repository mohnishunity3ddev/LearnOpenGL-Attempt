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
        std::cout << "Vertex Shader Compile Error: " << infoLog << "\n";
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
        std::cout << "Vertex Shader Compile Error: " << infoLog << "\n";
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

void Shader::use() {
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
#include "texture.h"
#include <project_paths.h>
#include <shader/shader.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

Texture::Texture(
        const char* textureName, 
        GLenum texture_type, 
        bool flipImage,
        GLint internalImageFormat,
        GLenum actualImageFormat,
        GLint sWrapMode, 
        GLint tWrapMode, 
        GLint minFilterMethod, 
        GLint magFilterMethod,
        bool useMipmap) {

    texture_path = texture_dir_path;
    texture_path.append(textureName);

    if(flipImage) {
        stbi_set_flip_vertically_on_load(true);
    }

    pixelData = stbi_load(texture_path.c_str(), &width, &height, &nrChannels, 0);

    if(pixelData) {
        std::cout << textureName << " loaded successfully.\n"; 
    } else {
        std::cout << "There was a problem loading " << textureName << ". Aborting...";
        exit(EXIT_FAILURE);
    }

    this->texture_type = texture_type;

    glGenTextures(1, &this->texture_handle);
    glBindTexture(this->texture_type, this->texture_handle);
    glTexParameteri(this->texture_type, GL_TEXTURE_WRAP_S, sWrapMode);
    glTexParameteri(this->texture_type, GL_TEXTURE_WRAP_T, tWrapMode);
    glTexParameteri(this->texture_type, GL_TEXTURE_MIN_FILTER, minFilterMethod);
    glTexParameteri(this->texture_type, GL_TEXTURE_MAG_FILTER, magFilterMethod);

    glTexImage2D(this->texture_type, 0, internalImageFormat, width, height, 0, actualImageFormat, GL_UNSIGNED_BYTE, pixelData);
    if(useMipmap) {
        glGenerateMipmap(this->texture_type);
    }

    glBindTexture(this->texture_type, 0);
}

void Texture::bindToTextureUnit(unsigned int textureUnit) const {
    if(texture_handle != UINT_MAX) {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(this->texture_type, this->texture_handle);
    }
}

Texture::~Texture() {
    if(pixelData) {
        stbi_image_free(pixelData);
        std::cout << this->texture_path << " freed.\n";
    }
}
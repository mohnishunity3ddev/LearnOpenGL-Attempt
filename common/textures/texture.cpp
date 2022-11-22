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

Texture::Texture(unsigned int screenWidth, unsigned int screenHeight) {
    createTextureAttachment(screenWidth, screenHeight);
}

void Texture::createTextureAttachment(unsigned int width, unsigned int height) {
    // Create default texture.
    glGenTextures(1, &texture_handle);
    this->texture_type = GL_TEXTURE_2D;
    glBindTexture(this->texture_type, this->texture_handle);

    glTexImage2D(this->texture_type, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTextureParameteri(texture_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(texture_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::Texture(const char *textureName, bool gamma, bool textureShouldWrap) { 
    texture_path = texture_dir_path;
    texture_path.append(textureName);
    glGenTextures(1, &texture_handle);

    stbi_set_flip_vertically_on_load(true);

    pixelData = stbi_load(texture_path.c_str(), &width, &height, &nrChannels, 0);
    if(pixelData) {
        GLenum internalFormat;
        GLenum dataFormat;
        if(nrChannels == 1) {
            internalFormat = dataFormat = GL_RED;
        } else if (nrChannels == 3) {
            internalFormat = gamma ? GL_SRGB : GL_RGB;
            dataFormat = GL_RGB;
        } else if(nrChannels == 4) {
            internalFormat = gamma ? GL_SRGB_ALPHA : GL_RGBA;
            dataFormat = GL_RGBA;
        }

        this->texture_type = GL_TEXTURE_2D;
        glBindTexture(this->texture_type, texture_handle);
        glTexImage2D(this->texture_type, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, pixelData);
        glGenerateMipmap(this->texture_type);

        GLint wrapParam = textureShouldWrap ? GL_REPEAT : GL_CLAMP_TO_EDGE;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapParam);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapParam);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        std::cout << "Texture failed to load at path: " << texture_path << std::endl;
        stbi_image_free(pixelData);
        pixelData = 0;
    }
}

void Texture::bindToTextureUnit(unsigned int textureUnit) const {
    if(texture_handle != UINT_MAX) {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(this->texture_type, this->texture_handle);
    } else {
        std::cout << "Texture not set correctly. Could not bind it.\n";
    }
}

Texture::~Texture() {
    if(pixelData) {
        stbi_image_free(pixelData);
        pixelData = 0;
        std::cout << this->texture_path << " freed.\n";
    }
}

unsigned int Texture::loadCubemap(std::vector<std::string> faces, bool createDefaultCubemap) {
    if(createDefaultCubemap) {
        faces = std::vector<std::string> {
            "skybox/right.jpg",
            "skybox/left.jpg",
            "skybox/top.jpg",
            "skybox/bottom.jpg",
            "skybox/front.jpg",
            "skybox/back.jpg"
        };
    }

    stbi_set_flip_vertically_on_load(false);
    
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        std::string texPath = texture_dir_path;
        texPath.append("cubemaps/");
        texPath.append(faces[i]);
        unsigned char *data = stbi_load(texPath.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    stbi_set_flip_vertically_on_load(true);
    return textureID;
}

void Texture::createDefaultTexture(unsigned int width, unsigned int height, GLint format, unsigned int *texture) {
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

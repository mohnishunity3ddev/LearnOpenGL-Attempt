#ifndef __JHELPERS_H
#define __JHELPERS_H

#include <glad/glad.h>
#include <stb_image.h>

#include <string>
#include <iostream>

static unsigned int TextureFromFile(const char* path, const std::string &directory, bool gamma = false) {
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if(data) {
        GLenum format;
        if(nrComponents == 1) {
            format = GL_RED;
        } else if(nrComponents == 3) {
            format = GL_RGB;
        } else if (nrComponents == 4) {
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_REPEAT);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << ".\n";
        stbi_image_free(data);
    }

    return textureID;
}

#endif
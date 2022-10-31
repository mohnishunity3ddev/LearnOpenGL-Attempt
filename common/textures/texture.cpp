#include "texture.h"
#include "../project_paths.h"
#include <iostream>

void resolveTexturePath(const char* texture_name);

Texture::Texture(const char* texture_name) {
    texture_path = texture_dir_path;
    texture_path.append(texture_name);
    std::cout << "Texture path is: " << texture_path << "\n";
}

Texture::~Texture() {

}
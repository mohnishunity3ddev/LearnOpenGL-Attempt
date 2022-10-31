#pragma once
#include <string>

class Texture {
public:
    Texture(const char* texture_name);
    ~Texture();
private:
    std::string texture_path;
};
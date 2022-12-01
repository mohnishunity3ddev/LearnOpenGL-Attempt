#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>

class Sdf {
public:
    Sdf(const std::string& fontFile, const int fontSize, const int upscaleResolution, const int codepoint);
    ~Sdf() { free(); }
    void generateCodepointBitmap(const int codepoint);
private:
    void free();
    std::string fontPath;
    unsigned int fontSize;
    unsigned int upscaleResolution;
    FT_Library ft;
    FT_Face face;
};
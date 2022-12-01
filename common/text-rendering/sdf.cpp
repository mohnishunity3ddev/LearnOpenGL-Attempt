#include "sdf.h"

#include <iostream>
#include <project_paths.h>
#include <textures/texture.h>

// Usable Font Names:
// Caskaydia Cove Mono
// JetBrains Mono
// Ubuntu Mono

static float mapRange(float val, float in_min, float in_max, float out_min, float out_max);
static float findNearestPixel(int upscaledPixelX, int upscaledPixelY, unsigned char* upscaledGlyphData, 
                int upscaledGlyphWidth, int upscaledGlyphHeight, int spread);

Sdf::Sdf(const std::string& fontFile, const int fontSize,  const int upscaleResolution, const int codepoint) {

    // Initialize Library
    if(FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE::Could not init Freetype Library.\n";
        exit(EXIT_FAILURE);
    }

    fontPath = fonts_dir_path;
    fontPath.append(fontFile);
    if(FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;  
        exit(EXIT_FAILURE);
    }

    this->fontSize = fontSize;
    this->upscaleResolution = upscaleResolution;
    // Pixel_Width of 0 means that ft decides on the font width dynamically based on the given height.

    generateCodepointBitmap(codepoint);
}

void Sdf::generateCodepointBitmap(const int codepoint) {

    int padding = 15;
    int upscaleResolution = this->upscaleResolution;
    int spread = upscaleResolution / 2;

    // we load the glyph with upscale resolution.
    FT_Set_Pixel_Sizes(face, 0, upscaleResolution);

    if(FT_Load_Char(face, (char)codepoint, FT_LOAD_RENDER)) {
        std::cout << "Could not load font character: " << (char)codepoint << "\n";
        exit(EXIT_FAILURE);
    }

    FT_GlyphSlot g = face->glyph;
    auto upscaledGlyphWidth   = g->bitmap.width;
    auto upscaledGlyphHeight  = g->bitmap.rows;
    auto upscaledGlyphData = g->bitmap.buffer;

    std::cout << "Glyph Width: "  << upscaledGlyphWidth  << "\n";
    std::cout << "Glyph Height: " << upscaledGlyphHeight << "\n";

    // The glyph width will also be smaller than the provided font size to
    // freetype. This is because the characters don't need that much space to
    // fit.
    float widthScale   = (float)upscaledGlyphWidth  / (float)upscaleResolution;
    float heightScale  = (float)upscaledGlyphHeight / (float)upscaleResolution;
    // Scaling the actual font size by the same scale that the upscale
    // resolution was scaled by above.
    int sdfCharacterWidth = (int)((float)fontSize * widthScale);
    int sdfCharacterHeight = (int)((float)fontSize * heightScale);
    // The actual width of the bitmap we will create which will be the SDF
    // version.
    int sdfBitmapWidth = sdfCharacterWidth + padding * 2;
    int sdfBitmapHeight = sdfCharacterHeight + padding * 2;
    // The glyphWidth is the width of the upscaled font. characterWidth is the
    // width of the font that the user has given. This is used to map from
    // pixels in our downscaled user coordinate system to the upscaled
    // coordinate system.
    float bitmapScaleX = (float)upscaledGlyphWidth  / (float)sdfCharacterWidth;
    float bitmapScaleY = (float)upscaledGlyphHeight / (float)sdfCharacterHeight;
    int *sdfBitmap = new int[sdfBitmapWidth * sdfBitmapHeight];

    for(int y = -padding; y < sdfCharacterHeight + padding; ++y) {
        for(int x = -padding; x < sdfCharacterWidth + padding; ++x) {
            // current X-direction pixel in the upscaled version of this font.
            int upscaledPixelX = (int)mapRange(x, -padding, sdfCharacterWidth + padding, 
                            -padding * bitmapScaleX, (sdfCharacterWidth + padding) * bitmapScaleX);
            // current Y-direction pixel in the upscaled version of this font.
            int upscaledPixelY = (int)mapRange(y, -padding, sdfCharacterHeight + padding, 
                            -padding * bitmapScaleY, (sdfCharacterHeight + padding) * bitmapScaleY);
            
            float val = findNearestPixel(upscaledPixelX, upscaledPixelY, upscaledGlyphData, 
                            upscaledGlyphWidth, upscaledGlyphHeight, spread);

            sdfBitmap[(x + padding) + ((y + padding) * sdfBitmapWidth)] = (int)(val * 255.0f);
        }
    }     
    
    unsigned char* sdfImageData = new unsigned char[sdfBitmapWidth * sdfBitmapHeight * 4];
    unsigned int *sdfImgPointer = reinterpret_cast<unsigned int*>(sdfImageData);
    auto *gP = sdfBitmap;
    int x = 0, y = 0;
    while(true) {
        int b = *(gP++) & 0xFF;
        int argb = (255 << 24) | (b << 16) | (b << 8) | b;
        sdfImgPointer[y * sdfBitmapWidth + x] = argb;
        if(x >= sdfBitmapWidth) {
            x = 0;
            y++;
        }
        if(y >= sdfBitmapHeight) {
            break;
        }
        x++;
    }

    Texture::save_image("fontTexture", sdfBitmapWidth, sdfBitmapHeight, 4, sdfImageData);
}

void Sdf::free() {
    FT_Done_Face(this->face);
    FT_Done_FreeType(this->ft);
}

// NOTE: Map from [in_min, in_max] to a range [out_min, out_max].
static float mapRange(float val, float in_min, float in_max, float out_min, float out_max) {

    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;   
}

static int getPixel(int x, int y, unsigned char* bitmap, int width, int height) {
    
    if(x >= 0 && x < width && y >= 0 && y < height) {
        return (bitmap[y * width + x] & 0xFF) == 0 ? 0 : 1;
    }
    return 0;
}

static float findNearestPixel(
    int pixelX, int pixelY, unsigned char* bitmap, 
    int width, int height, int spread) {
    
    int state = getPixel(pixelX, pixelY, bitmap, width, height);
    int minX = pixelX - spread;
    int maxX = pixelX + spread;
    int minY = pixelY - spread;
    int maxY = pixelY + spread;

    float minDistance = spread * spread;
    for(int y = minY; y < maxY; ++y) {
        for(int x = minX; x < maxX; ++x) {
            int pixelState = getPixel(x, y, bitmap, width, height);
            float dxSquared = (x - pixelX) * (x - pixelX);
            float dySquared = (y - pixelY) * (y - pixelY);
            float distanceSquared = dxSquared + dySquared;
            if(pixelState != state) {
                minDistance = std::min(minDistance, distanceSquared);
            }
        }
    }
    
    minDistance = (float)std::sqrt(minDistance);
    // mapping to range [0, 1]
    float output = (minDistance - 0.5f) / (spread - 0.5f);
    // in range [-1. 1]
    output *= state == 0 ? -1 : 1;

    // Bring again in range [0, 1]
    return (output + 1) * 0.5f;
}
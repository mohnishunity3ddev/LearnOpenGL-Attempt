#pragma once
#include <string>
#include <vector>
#include <glad/glad.h>


class Texture {
public:
    /**
     * @brief Construct a new Texture object
     * 
     * @param textureName The name of the texture file.
     * @param textureType The type of texture. GL_TEXTURE_2D / GL_TEXTURE_3D.
     * @param internalImageFormat The format of the image we are using internally here inside OpenGL
     * @param actualImageFormat The format of the actual image. For PNG images it is GL_RGBA, for JPG images it is GL_RGB 
     * @param sWrapMode Texture Wrapping Mode in the S/X Direction. Can be GL_REPEAT / GL_MIRRORED_REPEAT / GL_CLAMP_TO_EDGE / GL_CLAMP_TO_BORDER
     * @param tWrapMode Texture Wrapping Mode in the T/Y Direction. Can be GL_REPEAT / GL_MIRRORED_REPEAT / GL_CLAMP_TO_EDGE / GL_CLAMP_TO_BORDER
     * @param minFilterMethod Method to use for downsampled images. can be GL_NEAREST_MIPMAP_NEAREST / GL_LINEAR_MIPMAP_NEAREST / GL_NEAREST_MIPMAP_LINEAR / GL_LINEAR_MIPMAP_LINEAR
     * @param magFilterMethod Method to use for downsampled images. can be GL_LINEAR / GL_NEAREST. @note: Mipmap filters are not allowed here. Since mipmaps only apply for downsampled images. 
     * @param useMipmap Whether to generate mipmaps for this texture.
     */
    Texture(const char* textureName, GLenum textureType,  bool flipImage,
            GLint internalImageFormat, GLenum actualImageFormat, 
            GLint sWrapMode, GLint tWrapMode, 
            GLint minFilterMethod, GLint magFilterMethod, 
            bool useMipmap);

    Texture(const char *textureName, bool flipImage = true, bool gamma = false, bool textureShouldWrap = true);
    Texture(unsigned int screenWidth, unsigned int screen);
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture(Texture&&) = delete;
    Texture& operator=(Texture&&) = delete;

    ~Texture();

    /** 
     * @brief Bind the texture. 
     * @param textureUnit Specifies to OpenGL which active texture unit this texture is bound to. Texture Units are required to be set to shaders prior to this call.
     * So this texture will be bound to the texture unit already bound to the shader. This is how shaders get to know which textures are connected to sampler2Ds inside the shaders. 
    */
    void bindToTextureUnit(unsigned int textureUnit) const;

    int getWidth() const noexcept { return width; }
    int getHeight() const noexcept { return height; }
    int getChannels() const noexcept { return nrChannels; }
    unsigned int texture_handle = UINT_MAX;

    static unsigned int loadCubemap(std::vector<std::string> faces, bool createDefaultCubemap = false);
    static void createDefaultTexture(unsigned int width, unsigned int height, GLint format, unsigned int *texture);
private:
    unsigned char* pixelData = nullptr;
    std::string texture_path;
    GLenum texture_type; 
    int width = -1, height = -1, nrChannels = -1;

    void createTextureAttachment(unsigned int width, unsigned int height);
};
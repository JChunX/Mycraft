#ifndef TEXTURE_H
#define TEXTURE_H

#include"Shader.h"
#include"stb/stb_image.h"
#include <glad/glad.h>

class Texture
{
public:
    // Reference ID of the Texture
    GLuint ID;
    // Texture type
    GLenum m_textype;
    // Constructor
    Texture(GLenum textype, const char* path, GLenum format, GLenum pixelType);
    void texUnit(Shader& shader, const char* uniform, GLuint unit);
    // Bind and unbinding
    void Bind();
    void Unbind();
    // Deletes the Texture
    void Delete();

private:
    void ConfigureTexParameters();
};

#endif
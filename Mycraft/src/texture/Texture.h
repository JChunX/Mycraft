#ifndef TEXTURE_H
#define TEXTURE_H

#include"Shader.h"
#include"stb/stb_image.h"
#include <glad/glad.h>

class Texture
{
protected:
    GLuint ID;
    GLenum m_textype;

    Texture(GLenum textype): m_textype(textype)
    {
        glGenTextures(1, &ID);
        Bind();
    }
    virtual ~Texture() = default;

    virtual void ConfigureTexParameters() = 0;

public:
    void texUnit(Shader& shader, const char* uniform, GLuint unit);

    void Bind() { 
        glBindTexture(m_textype, ID);
    }

    void Unbind() {
        glBindTexture(m_textype, 0);
    }

    void Delete() {
        glDeleteTextures(1, &ID);
    }


};

#endif
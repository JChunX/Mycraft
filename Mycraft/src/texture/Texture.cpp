#include "Texture.h"
#include<iostream>

Texture::Texture(GLenum textype, const char* path, GLenum format, GLenum pixelType)
	: m_textype(m_textype)
{
    glGenTextures(1, &ID);
    Bind();

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);  
	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0); 

	if (!data)
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	glTexImage2D(m_textype, 0, GL_RGBA, width, height, 0, format, pixelType, data);
	stbi_image_free(data);

	ConfigureTexParameters();

    Unbind();
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit) 
{
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	shader.Activate();
	glUniform1i(texUni, unit);
}

void Texture::Bind()
{
    glBindTexture(m_textype, ID);
}

void Texture::Unbind()
{
    glBindTexture(m_textype, 0);
}

void Texture::Delete()
{
    glDeleteTextures(1, &ID);
}

void Texture::ConfigureTexParameters()
{
	switch (m_textype)
	{
	case GL_TEXTURE_2D:
		glGenerateMipmap(m_textype);
		glTexParameteri(m_textype, GL_TEXTURE_WRAP_S, GL_REPEAT);	
		glTexParameteri(m_textype, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// https://gamedev.stackexchange.com/questions/174205/removing-texture-wrapping-artifacts-in-opengles
		glTexParameteri(m_textype, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(m_textype, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(m_textype, GL_TEXTURE_MAX_LEVEL, 4);
		break;
	case GL_TEXTURE_CUBE_MAP:
		glTexParameteri(m_textype, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(m_textype, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(m_textype, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(m_textype, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(m_textype, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
}
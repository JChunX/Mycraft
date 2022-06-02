#include "Texture.h"
#include"stb/stb_image.h"

#include<iostream>

Texture::Texture(GLenum texType, const char* path, GLenum format, GLenum pixelType)
{
	this->texType = texType;
    glGenTextures(1, &ID);
    Bind();

    // Set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Load image
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);  
	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0); 

	// Error check if the image fails to load
	if (!data)
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	// Generate the texture
	// 1. texture target
	// 2. mipmap level
	// 3. texture format
	// 4. width
	// 5. height
	// 6. legacy
	// 7-8. data type
	// 9. the data
	// our texture source png is RGBA!
	glTexImage2D(texType, 0, GL_RGBA, width, height, 0, format, pixelType, data);
	// Generates MipMaps
	glGenerateMipmap(texType);
	stbi_image_free(data);
    Unbind();
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit) 
{
    // Gets ID of uniform called "texture"
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	// Shader needs to be activated before changing the value of a uniform
	shader.Activate();
	// Sets the value of the uniform
	glUniform1i(texUni, unit);
}

void Texture::Bind()
{
    glBindTexture(texType, ID);
}

void Texture::Unbind()
{
    glBindTexture(texType, 0);
}

void Texture::Delete()
{
    glDeleteTextures(1, &ID);
}
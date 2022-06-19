#include "BlockTexture.h"

BlockTexture::BlockTexture(const char* path)
    : Texture(GL_TEXTURE_2D)
{

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);  
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0); 

    if (!data)
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    glTexImage2D(m_textype, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
    ConfigureTexParameters();
    
}

void BlockTexture::ConfigureTexParameters()
{
    glGenerateMipmap(m_textype);
    glTexParameteri(m_textype, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(m_textype, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // https://gamedev.stackexchange.com/questions/174205/removing-texture-wrapping-artifacts-in-opengles
    glTexParameteri(m_textype, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(m_textype, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(m_textype, GL_TEXTURE_MAX_LEVEL, 4);
}
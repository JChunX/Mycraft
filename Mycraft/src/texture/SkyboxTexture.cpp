#include "SkyboxTexture.h"
#include <vector>

SkyboxTexture::SkyboxTexture()
    : Texture(GL_TEXTURE_CUBE_MAP)
{
    std::vector<std::string> faces = {"resources/skybox_right.jpg", 
                                    "resources/skybox_left.jpg", 
                                    "resources/skybox_top.jpg", 
                                    "resources/skybox_bottom.jpg", 
                                    "resources/skybox_front.jpg", 
                                    "resources/skybox_back.jpg"};
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        stbi_set_flip_vertically_on_load(false);
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    ConfigureTexParameters();
}

void SkyboxTexture::ConfigureTexParameters()
{
    glTexParameteri(m_textype, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(m_textype, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(m_textype, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(m_textype, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(m_textype, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
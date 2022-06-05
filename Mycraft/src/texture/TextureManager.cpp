#include"TextureManager.h"

TextureManager::TextureManager() 
    : m_texture(Texture(GL_TEXTURE_2D, 
                "textures/default_pack.png", 
                GL_RGBA, 
                GL_UNSIGNED_BYTE))
{
    BindTexture();
}

TextureManager::~TextureManager()
{

}

void TextureManager::LoadTextureOffsets(std::string path)
{

}

glm::vec2 TextureManager::RetrieveTextureOffsets(BlockType type, BlockFace face)
{
    return glm::vec2(16.0f/16.0f, 16.0f/16.0f);
}

void TextureManager::BindTexture()
{
    m_texture.Bind();
}
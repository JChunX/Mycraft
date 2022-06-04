#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include"Texture.h"
#include"Block.h"
#include<utility>
#include<map>
#include<string>


using TextureOffsets = std::pair<float, float>;

struct TextureCoords
{
    TextureOffsets offsets_side;
    TextureOffsets offsets_top;
    TextureOffsets offsets_bottom;
};

class TextureManager
{
public:
    Texture m_texture;
    std::map<BlockType, TextureCoords> m_texture_coords; 
    TextureManager();
    ~TextureManager();
    void LoadTextureOffsets(std::string path);
    glm::vec2 RetrieveTextureOffsets(BlockType type, BlockFace face);
    void BindTexture();
};

#endif // TEXTUREMANAGER_H
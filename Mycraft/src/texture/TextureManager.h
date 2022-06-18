#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include"Texture.h"
#include"Block.h"
#include"nlohmann/json.hpp"
#include<utility>
#include<map>
#include<string>


using TextureOffsets = std::pair<float, float>;
using json = nlohmann::json;

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
    Texture m_skybox;
    std::map<std::string, TextureCoords> m_texture_coords; 
    std::vector<float> texture_recolor_map;

    TextureManager();
    ~TextureManager();

    std::pair<float,float> RetrieveTextureOffsets(BlockType type, BlockFace face);
    glm::vec4 RetrieveTextureRecolor(BlockType type, BlockFace face, int temperature, int moisture);

private:
    void LoadTextureRecolorMap(std::string path);
    void LoadTextureOffsets(std::string path);
    void BindTextures();
};

#endif // TEXTUREMANAGER_H
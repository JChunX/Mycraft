#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include"BlockTexture.h"
#include"SkyboxTexture.h"
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
    std::unique_ptr<BlockTexture> m_block_texture;
    std::unique_ptr<SkyboxTexture> m_skybox_texture;
    std::map<std::string, TextureCoords> m_block_texture_coords; 
    std::vector<float> m_block_texture_recolor_map;

    TextureManager();
    ~TextureManager();

    std::pair<float,float> RetrieveBlockTextureOffsets(BlockType type, BlockFace face);
    glm::vec4 RetrieveBlockTextureRecolor(BlockType type, BlockFace face, int temperature, int moisture);
    int RetrieveCubeMapTextureOffsets(BlockFace face);

    void BindTexture(Texture& texture);

private:
    void LoadBlockTextureRecolorMap(std::string path);
    void LoadBlockTextureOffsets(std::string path);
};

#endif // TEXTUREMANAGER_H
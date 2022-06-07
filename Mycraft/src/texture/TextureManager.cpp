#include "TextureManager.h"

TextureManager::TextureManager() 
    : m_texture(Texture(GL_TEXTURE_2D, 
                "textures/default_pack.png", 
                GL_RGBA, 
                GL_UNSIGNED_BYTE))
{
    LoadTextureOffsets("textures/default_pack_offsets.json");
    BindTexture();
}

TextureManager::~TextureManager()
{

}

void TextureManager::LoadTextureOffsets(std::string path)
{
    std::ifstream file(path);
    //std::cout << file.rdbuf();

    json j;
    file >> j;

    for (auto& [key, value] : j.items())
    {
        m_texture_coords[key] = {
            {value["side"][0], value["side"][1]},
            {value["top"][0], value["top"][1]},
            {value["bottom"][0], value["bottom"][1]}
        };
    }

    file.close();

    //print texture coords
    for (auto& [key, value] : m_texture_coords)
    {
        std::cout << "BlockType: " << key << std::endl;
        std::cout << "Side: " << value.offsets_side.first << ", " << value.offsets_side.second << std::endl;
        std::cout << "Top: " << value.offsets_top.first << ", " << value.offsets_top.second << std::endl;
        std::cout << "Bottom: " << value.offsets_bottom.first << ", " << value.offsets_bottom.second << std::endl;
    }
}

std::pair<float,float> TextureManager::RetrieveTextureOffsets(BlockType type, BlockFace face)
{
    TextureCoords texture_coords;
    switch (type)
    {
    case BlockType::GRASS:
        texture_coords = m_texture_coords["grass"];
        break;
    case BlockType::DIRT:
        texture_coords = m_texture_coords["dirt"];
        break;
    case BlockType::STONE:
        texture_coords = m_texture_coords["stone"];
        break;
    case BlockType::SAND:   
        texture_coords = m_texture_coords["sand"];
        break;
    default:
        texture_coords = m_texture_coords["grass"];
        break;
    }
    switch (face)
    {
    case BlockFace::TOP:
        return texture_coords.offsets_top;
    case BlockFace::BOTTOM:
        return texture_coords.offsets_bottom;
    case BlockFace::LEFT:
    case BlockFace::RIGHT:
    case BlockFace::FRONT:
    case BlockFace::BACK:
        return texture_coords.offsets_side;
    default:
        return texture_coords.offsets_bottom;
    }
}

void TextureManager::BindTexture()
{
    m_texture.Bind();
}
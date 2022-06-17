#include"TextureManager.h"

TextureManager::TextureManager() 
    : m_texture(Texture(GL_TEXTURE_2D, 
                "resources/default_pack.png", 
                GL_RGBA, 
                GL_UNSIGNED_BYTE))
{
    LoadTextureOffsets("resources/default_pack_offsets.json");
    LoadTextureRecolorMap("resources/biome_colormap.png");
    BindTexture();
}

TextureManager::~TextureManager()
{

}

void TextureManager::LoadTextureRecolorMap(std::string path)
{
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path.data(), &width, &height, &nrChannels, 4); 
    if (data)
    {
        for (int i = 0; i < width * height * 4; i += 4)
        {
            unsigned char r = data[i];
            unsigned char g = data[i + 1];
            unsigned char b = data[i + 2];
            unsigned char a = data[i + 3];
            texture_recolor_map.push_back(r / 255.0f);
            texture_recolor_map.push_back(g / 255.0f);
            texture_recolor_map.push_back(b / 255.0f);
            texture_recolor_map.push_back(a / 255.0f);
        }
    }
    else
    {
        std::cout << "Failed to load recolor map" << std::endl;
    }
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

glm::vec4 TextureManager::RetrieveTextureRecolor(BlockType type, BlockFace face, int temperature, int moisture)
{
    glm::vec4 color;

    switch (face)
    {
    case BlockFace::TOP:
        color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        break;
    case BlockFace::BOTTOM:
        color = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
        break;
    case BlockFace::RIGHT:
    case BlockFace::LEFT:
    case BlockFace::FRONT:
    case BlockFace::BACK:
        color = glm::vec4(0.85f, 0.85f, 0.85f, 1.0f);
        break;
    default:
        color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        break;
    }

    switch (type)
    {
        case BlockType::GRASS:
            break;
        default:
            return color;
    }

    temperature = std::clamp(temperature, 0, 31);
    moisture = std::clamp(moisture, 0, 31);

    int index = temperature + moisture * 32;

    color *= glm::vec4(texture_recolor_map[index * 4], texture_recolor_map[index * 4 + 1], texture_recolor_map[index * 4 + 2], texture_recolor_map[index * 4 + 3]);

    return color;
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
    case BlockType::WATER:
        texture_coords = m_texture_coords["water"];
        break;
    case BlockType::GRAVEL:
        texture_coords = m_texture_coords["gravel"];
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
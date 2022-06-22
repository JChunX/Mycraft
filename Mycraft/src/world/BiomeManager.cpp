#include "BiomeManager.h"

BiomeType BiomeManager::GetBiomeTypeFromParams(float height, float moisture, float temperature)
{
    // height: float from 0 to WORLD_HEIGHT
    // moisture: float from 0 to 31
    // temperature: float from 0 to 31

    if (height < WATER_LEVEL)
    {
        return BiomeType::OCEAN;
    }
    if (height < WORLD_FLOOR + 50)
    {
        if (temperature < 6) {
            if (moisture < 15) {
                return BiomeType::TUNDRA;
            }
            else {
                return BiomeType::TAIGA;
            }
        }
        else {
            if (moisture < 6) {
                return BiomeType::DESERT;
            }
            else if (moisture < 20) {
                return BiomeType::PLAINS;
            }
            else {
                return BiomeType::FOREST;
            }
        }

    }
    else
    {
        return BiomeType::MOUNTAIN;
    }
}

std::vector<BlockType> BiomeManager::GetBlockLayers(BiomeType type)
{

    switch (type)
    {
    case BiomeType::MOUNTAIN:
        return mountain_layers;
    case BiomeType::PLAINS:
        return plains_layers;

    case BiomeType::DESERT:
        return desert_layers;
    
    case BiomeType::OCEAN:
        return ocean_layers;

    case BiomeType::FOREST:
        return forest_layers;

    case BiomeType::TUNDRA:
        return tundra_layers;

    case BiomeType::TAIGA:
        return taiga_layers;
    
    default:
        return plains_layers;
    }
}

void BiomeManager::GenerateBiomes(Chunk& chunk, int ymin, int ymax)
{
    //std::cout << "Generating biome..." << chunk.m_x << " " << chunk.m_z << std::endl;
    std::shared_ptr<Terrain> terrain = chunk.m_terrain;

    for (int z = 0; z < CHUNK_SIZE; z++) 
    {
        for (int x = 0; x < CHUNK_SIZE; x++) 
        {
            float height = terrain->heightmap[terrain->GetIndex(x, z)];
            float moisture = terrain->moisturemap[terrain->GetIndex(x, z)];
            float temperature = terrain->temperaturemap[terrain->GetIndex(x, z)];
            
            std::vector<BlockType> layers = GetBlockLayers(GetBiomeTypeFromParams(height, moisture, temperature));

            for (int y = ymin; y < ymax; y++) 
            {
                glm::vec3 block_position = glm::vec3(chunk.m_x+x,y,chunk.m_z+z);
                if (y == 0) 
                {
                    chunk.m_chunkdata.emplace_back(BlockType::BEDROCK, block_position);
                }
                else if (y <= height - 2) {
                    // rock type
                    chunk.m_chunkdata.emplace_back(layers[0], block_position);
                }
                else if (y <= height - 1) {
                    // below surface type
                    chunk.m_chunkdata.emplace_back(layers[1], block_position);
                }
                else if (y <= height) {
                    if (y == WATER_LEVEL || y == WATER_LEVEL + 1) {
                        // water-adjacent block type
                        chunk.m_chunkdata.emplace_back(layers[2], block_position);
                    }
                    else if (y < WATER_LEVEL) {
                        chunk.m_chunkdata.emplace_back(layers[1], block_position);
                    }
                    else {
                        // surface block type
                        chunk.m_chunkdata.emplace_back(layers[3], block_position);
                    }
                }
                else if (y <= WATER_LEVEL){
                    // "water" block type
                    chunk.m_chunkdata.emplace_back(layers[4], block_position);
                }
                else {
                    chunk.m_chunkdata.emplace_back(BlockType::AIR, block_position);
                }
            }
        }
    }
}
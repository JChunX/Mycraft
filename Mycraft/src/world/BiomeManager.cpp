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
        return {
            BlockType::STONE, 
            BlockType::STONE, 
            BlockType::GRAVEL, 
            BlockType::STONE, 
            BlockType::WATER
        };
    case BiomeType::PLAINS:
        return {
            BlockType::STONE, 
            BlockType::DIRT, 
            BlockType::SAND, 
            BlockType::GRASS, 
            BlockType::WATER
        };

    case BiomeType::DESERT:
        return {
            BlockType::STONE, 
            BlockType::SAND, 
            BlockType::SAND, 
            BlockType::SAND, 
            BlockType::WATER
        };
    
    case BiomeType::OCEAN:
        return {
            BlockType::STONE, 
            BlockType::GRAVEL, 
            BlockType::SAND, 
            BlockType::GRASS, 
            BlockType::WATER
        };

    case BiomeType::FOREST:
        return {
            BlockType::STONE, 
            BlockType::DIRT, 
            BlockType::SAND, 
            BlockType::GRASS, 
            BlockType::WATER
        };

    case BiomeType::TUNDRA:
        return {
            BlockType::STONE, 
            BlockType::DIRT, 
            BlockType::GRAVEL, 
            BlockType::GRASS, 
            BlockType::WATER
        };

    case BiomeType::TAIGA:
        return {
            BlockType::STONE, 
            BlockType::DIRT, 
            BlockType::GRAVEL, 
            BlockType::GRASS, 
            BlockType::WATER
        };
    
    default:
        return {
            BlockType::STONE, 
            BlockType::DIRT, 
            BlockType::SAND, 
            BlockType::GRASS, 
            BlockType::WATER
        };
    }
}

void BiomeManager::GenerateBiomes(Chunk& chunk, int ymin, int ymax, std::vector<BlockType>& layers)
{
    for (int y = ymin; y < ymax; y++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                std::shared_ptr<Terrain> terrain = chunk.m_terrain;
                float height = terrain->heightmap[terrain->GetIndex(x, z)];
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
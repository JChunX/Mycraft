#ifndef BIOMEMANAGER_H
#define BIOMEMANAGER_H

#include <memory>
#include <vector>
#include "Block.h"
#include "Chunk.h"

class Chunk;

#define BLOCK_LAYERS 5

enum class BiomeType
{
    MOUNTAIN,
    PLAINS,
    DESERT,
    OCEAN,
    FOREST,
    TUNDRA,
    TAIGA
};

class BiomeManager
{
public:

    static inline std::vector<BlockType> mountain_layers = {
            BlockType::STONE, 
            BlockType::STONE, 
            BlockType::GRAVEL, 
            BlockType::STONE, 
            BlockType::WATER
        };

    static inline std::vector<BlockType> plains_layers = {
            BlockType::STONE, 
            BlockType::DIRT, 
            BlockType::SAND, 
            BlockType::GRASS, 
            BlockType::WATER
        };

    static inline std::vector<BlockType> desert_layers = {
            BlockType::STONE, 
            BlockType::SAND, 
            BlockType::SAND, 
            BlockType::SAND, 
            BlockType::WATER
        };
    
    static inline std::vector<BlockType> ocean_layers = {
            BlockType::STONE, 
            BlockType::GRAVEL, 
            BlockType::SAND, 
            BlockType::GRASS, 
            BlockType::WATER
        };
    
    static inline std::vector<BlockType> forest_layers = {
            BlockType::STONE, 
            BlockType::DIRT, 
            BlockType::SAND, 
            BlockType::GRASS, 
            BlockType::WATER
        };
    
    static inline std::vector<BlockType> tundra_layers = {
            BlockType::STONE, 
            BlockType::DIRT, 
            BlockType::GRAVEL, 
            BlockType::GRASS, 
            BlockType::WATER
        };

    static inline std::vector<BlockType> taiga_layers = {
            BlockType::STONE, 
            BlockType::DIRT, 
            BlockType::SAND, 
            BlockType::GRASS, 
            BlockType::WATER
        };

    static BiomeType GetBiomeTypeFromParams(float height, float moisture, float temperature);
    static std::vector<BlockType> GetBlockLayers(BiomeType type);
    static void GenerateBiomes(Chunk& chunk, int ymin, int ymax);
};

#endif // BIOMEMANAGER_H
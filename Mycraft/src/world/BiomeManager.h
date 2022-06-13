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
    static BiomeType GetBiomeTypeFromParams(float height, float moisture, float temperature);
    static std::vector<BlockType> GetBlockLayers(BiomeType type);
    static void GenerateBiomes(Chunk& chunk, int ymin, int ymax, std::vector<BlockType>& layers);
};

#endif // BIOMEMANAGER_H
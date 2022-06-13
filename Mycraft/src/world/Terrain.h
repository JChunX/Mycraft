#ifndef TERRAIN_H
#define TERRAIN_H

enum class BiomeType;

#include "GameParameters.h"
#include "BiomeManager.h"

struct Terrain 
{

    std::vector<float> heightmap;
    std::vector<float> moisturemap;
    std::vector<float> temperaturemap;

    BiomeType biome_type;

    int GetIndex(int x, int z) {
        return x + z * CHUNK_SIZE;
    }
};

#endif // TERRAIN_H
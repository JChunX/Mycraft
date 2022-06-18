#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include <numeric>
#include <memory>
#include "Terrain.h"
#include "BiomeManager.h"
#include "fastnoise/FastNoiseLite.h"

#define NUM_OCTAVES 6

struct PerlinParameters 
{
    float octaves[NUM_OCTAVES];
    float exp;
    float scale;
    float amp_mod;
    float offset;
};

struct TerrainParameters
{
    int seed;
    PerlinParameters height_perlin;
    PerlinParameters moisture_perlin;
    PerlinParameters temperature_perlin;
};

class TerrainGenerator
{
public:
    static constexpr TerrainParameters parameters = 
    {
        42690, 
        {{1.0f, 0.4f, 0.3f, 0.2f, 0.02f, 0.02f}, 
            1.0f, 0.5f, WORLD_HEIGHT/3.5f, WORLD_FLOOR-10.0f}, 
        {{1.0f, 0.6f, 0.4f, 0.2f, 0.1f, 0.1f}, 
            1.0f, 0.08f, 30.0f, -5.0f},
        {{1.0f, 0.6f, 0.4f, 0.2f, 0.1f, 0.1f}, 
            1.0f, 0.1f, 30.0f, 5.0f}
    };

    static std::shared_ptr<Terrain> GenerateTerrain(int x, int z);
};

#endif // TERRAINGENERATOR_H
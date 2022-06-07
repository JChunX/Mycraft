#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include "Terrain.h"
#include "fastnoise/FastNoiseLite.h"

#define NUM_HEIGHT_OCATVES 4

struct TerrainParameters
{
    int seed;
    float height_octaves[NUM_HEIGHT_OCATVES];
    float height_exp;

};

class TerrainGenerator
{
public:
    static constexpr TerrainParameters parameters = {12345, {1.0f, 0.6f, 0.4f, 0.2f}, 1.0f};

    static Terrain GenerateTerrain(int x, int z);
};

#endif // TERRAINGENERATOR_H
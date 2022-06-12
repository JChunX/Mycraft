#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include"Terrain.h"
#include"fastnoise/FastNoiseLite.h"

#define NUM_OCTAVES 4

struct PerlinParameters 
{
    float height_octaves[NUM_OCTAVES];
    float height_exp;
    float height_scale;
};

struct TerrainParameters
{
    int seed;
    //PerlinParameters height_params;
    //PerlinParameters moisture_params;
    //PerlinParameters temperature_params;
    float height_octaves[NUM_OCTAVES];
    float height_exp;
    float height_scale;
    float height_amplitude_mod;
};

class TerrainGenerator
{
public:
    static constexpr TerrainParameters parameters = 
    {
        12345, 
        {1.0f, 0.3f, 0.1f, 0.1f}, 
        1.2f,
        1.0f,
        9.0f
    };

    static Terrain GenerateTerrain(int x, int z);
};

#endif // TERRAINGENERATOR_H
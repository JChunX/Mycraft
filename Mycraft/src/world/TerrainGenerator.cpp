#include "TerrainGenerator.h"
#include <iostream> 

void GeneratePerlin(int xc, int zc, std::vector<float>& noisemap, int seed, PerlinParameters parameters);

std::shared_ptr<Terrain> TerrainGenerator::GenerateTerrain(int xc, int zc)
{
    //https://www.redblobgames.com/maps/terrain-from-noise/
    Terrain terrain;
    GeneratePerlin(xc, zc, terrain.heightmap, parameters.seed, parameters.height_perlin);
    GeneratePerlin(xc, zc, terrain.moisturemap, parameters.seed+1, parameters.moisture_perlin);
    GeneratePerlin(xc, zc, terrain.temperaturemap, parameters.seed+2, parameters.temperature_perlin);

    float avg_height = std::reduce(terrain.heightmap.begin(), terrain.heightmap.end()) / (CHUNK_SIZE * CHUNK_SIZE);
    float avg_moisture = std::reduce(terrain.moisturemap.begin(), terrain.moisturemap.end()) / (CHUNK_SIZE * CHUNK_SIZE);
    float avg_temperature = std::reduce(terrain.temperaturemap.begin(), terrain.temperaturemap.end()) / (CHUNK_SIZE * CHUNK_SIZE);


    terrain.biome_type = BiomeManager::GetBiomeTypeFromParams(avg_height, avg_moisture, avg_temperature);

    return std::make_shared<Terrain>(terrain);
}

void GeneratePerlin(int xc, int zc, std::vector<float>& noisemap, int seed, PerlinParameters parameters)
{
    FastNoiseLite noise_generator;
    noise_generator.SetSeed(seed);
    const float* octaves = parameters.octaves;
    float exp = parameters.exp;

    for (int z = 0; z < CHUNK_SIZE; z++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {

            float noise = 0.0f;
            float scale = parameters.scale;
            float octave_sum = 0.0f;
            for (int i = 0; i < NUM_OCTAVES; i++) {
                float octave = octaves[i];
                noise += octave * (noise_generator.GetNoise((float)(x+xc) * scale, (float)(z+zc) * scale) / 2.0 + 0.5);
                scale *= 2.0f;
                octave_sum += octave;
            }
            noise /= octave_sum;
            noise = pow(noise, exp);
            noise = noise * parameters.amp_mod + parameters.offset;
            std::cout << noise << std::endl;

            noisemap.push_back(noise);
        }   
    }
}

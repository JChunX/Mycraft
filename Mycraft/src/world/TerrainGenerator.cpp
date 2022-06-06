#include "TerrainGenerator.h"

Terrain TerrainGenerator::GenerateTerrain(int xc, int zc)
{
    //https://www.redblobgames.com/maps/terrain-from-noise/
    Terrain terrain;
    FastNoiseLite noise_generator;
    noise_generator.SetSeed(parameters.seed);
    const float* height_octaves = parameters.height_octaves;
    float height_exp = parameters.height_exp;

    for (int z = 0; z < CHUNK_SIZE; z++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {

            float height_noise = 0.0f;
            float scale = 1.0f;
            float octave_sum = 0.0f;
            for (int i = 0; i < NUM_HEIGHT_OCATVES; i++) {
                float octave_height = height_octaves[i];
                height_noise += octave_height * (noise_generator.GetNoise((float)(x+xc) * scale, (float)(z+zc) * scale))/2.0f + 0.5f;
                scale *= 2.0f;
                octave_sum += octave_height;
            }
            height_noise /= octave_sum;
            height_noise = pow(height_noise, height_exp);
            //std::cout << height_noise << std::endl;
            terrain.height[x][z] = height_noise;
        }
    }
    return terrain;

}

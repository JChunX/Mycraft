#ifndef TERRAIN_H
#define TERRAIN_H

#include "GameParameters.h"

struct Terrain 
{
    float height[CHUNK_SIZE][CHUNK_SIZE];
    float moisture[CHUNK_SIZE][CHUNK_SIZE];
    float temperature[CHUNK_SIZE][CHUNK_SIZE];

};

#endif // TERRAIN_H
#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include <future>
#include "Block.h"
#include "TerrainGenerator.h"
#include "Terrain.h"
#include "GameParameters.h"
#include "BiomeManager.h"

struct Terrain;

class Chunk 
{
public:
    Chunk(int x, int z);
    Chunk();
    ~Chunk();

    std::vector<Block> m_chunkdata;
    int m_x;
    int m_z;
    bool need_mesh_update;
    std::shared_ptr<Terrain> m_terrain;

    void Generate();
    void Update();
    void SetBlock(int x, int y, int z, Block& block);
    std::shared_ptr<Block> GetBlock(int x, int y, int z);
    BlockType GetBlockType(int x, int y, int z);
    int GetIndex(int x, int y, int z);
    glm::vec2 GetClimate(int x, int z);
    static bool IsInChunk(int x, int y, int z);
};

#endif // CHUNK_H
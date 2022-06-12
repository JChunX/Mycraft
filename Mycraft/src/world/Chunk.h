#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include <future>
#include "Block.h"
#include "TerrainGenerator.h"
#include "Terrain.h"
#include "GameParameters.h"

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
    Terrain m_terrain;

    void Generate();
    void Update();
    void SetBlock(int x, int y, int z, Block& block);
    Block* GetBlock(int x, int y, int z);
    BlockType GetBlockType(int x, int y, int z);
    int GetIndex(int x, int y, int z);
    static bool IsInChunk(int x, int y, int z);

private:
    void GenerateAux(int zmin, int zmax);
};

#endif // CHUNK_H
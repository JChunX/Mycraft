#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include "Constants.h"
#include "Block.h"
#include "TerrainGenerator.h"

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

    void Generate();
    void Update();
    void SetBlock(int x, int y, int z, Block& block);
    Block* GetBlock(int x, int y, int z);
    BlockType GetBlockType(int x, int y, int z);
    int GetIndex(int x, int y, int z);
    static bool IsInChunk(int x, int y, int z);
};

#endif // CHUNK_H
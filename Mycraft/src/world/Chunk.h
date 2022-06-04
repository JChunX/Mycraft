#ifndef CHUNK_H
#define CHUNK_H

#define CHUNK_SIZE 16
#define WORLD_HEIGHT 32

#include <vector>
#include "Block.h"

class Chunk 
{
public:
    Chunk(int x, int z);
    ~Chunk();

    std::vector<Block> m_chunkdata;
    int m_x;
    int m_z;

    void Generate();
    void Update();
    void SetBlock(int x, int y, int z, Block& block);
    Block GetBlock(int x, int y, int z);
    int GetIndex(int x, int y, int z);
    bool IsInChunk(int x, int y, int z);
};

#endif // CHUNK_H
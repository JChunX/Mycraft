#include"Chunk.h"

Chunk::Chunk(int x, int z)
    : m_x(x),
      m_z(z)
{
    m_chunkdata = std::vector<Block>(CHUNK_SIZE * CHUNK_SIZE * WORLD_HEIGHT);
    Generate();
}

void Chunk::Generate()
{

    for (int y=0; y<WORLD_HEIGHT; y++)
    {
        for (int x=0; x<CHUNK_SIZE; x++)
        {
            for (int z=0; z<CHUNK_SIZE; z++)
            {
                glm::vec3 block_position = glm::vec3(m_x+x,y,m_z+z);
                if (y < 100)
                {
                    m_chunkdata[GetIndex(x,y,z)] = Block(BlockType::GRASS, block_position);
                }
                else if (y < 100 + 3)
                {
                    m_chunkdata[GetIndex(x,y,z)] = Block(BlockType::GRASS, block_position);
                }
                else
                {
                    m_chunkdata[GetIndex(x,y,z)] = Block(BlockType::STONE, block_position);
                }
            }
        }
    }
}

void Chunk::Update()
{

}

void Chunk::SetBlock(int x, int y, int z, Block& block)
{   
    if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= WORLD_HEIGHT || z < 0 || z >= CHUNK_SIZE)
        return;
    m_chunkdata[GetIndex(x, y, z)] = block;
}

Block Chunk::GetBlock(int x, int y, int z)
{
    if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= WORLD_HEIGHT || z < 0 || z >= CHUNK_SIZE)
        return Block();
    return m_chunkdata[GetIndex(x, y, z)];
}

int Chunk::GetIndex(int x, int y, int z)
{
    return x + y * CHUNK_SIZE * CHUNK_SIZE + z * CHUNK_SIZE;
}

Chunk::~Chunk()
{

}
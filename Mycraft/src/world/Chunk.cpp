#include"Chunk.h"

Chunk::Chunk(int x, int z)
    : m_x(x),
      m_z(z),
      need_mesh_update(true),
      m_terrain(TerrainGenerator::GenerateTerrain(m_x, m_z))
{

    Generate();
}

Chunk::Chunk()
    : Chunk(0, 0)
{

}

Chunk::~Chunk()
{

}

void Chunk::Generate()
{
    BiomeManager::GenerateBiomes(*this, 0, WORLD_HEIGHT);
}

void Chunk::Update()
{

}

void Chunk::SetBlock(int x, int y, int z, Block& block)
{   
    if (!IsInChunk(x,y,z))
        return;
    m_chunkdata[GetIndex(x, y, z)] = block;
}

Block* Chunk::GetBlock(int x, int y, int z)
{
    if (!IsInChunk(x, y, z))
        return nullptr;
    return &(m_chunkdata[GetIndex(x, y, z)]);
}

BlockType Chunk::GetBlockType(int x, int y, int z)
{
    if (!IsInChunk(x, y, z))
        return BlockType::NONE;
    return m_chunkdata[GetIndex(x, y, z)].block_type;
}

int Chunk::GetIndex(int x, int y, int z)
{
    return y + x * WORLD_HEIGHT + z * WORLD_HEIGHT * CHUNK_SIZE;
}

bool Chunk::IsInChunk(int x, int y, int z)
{
    return x >= 0 && x < CHUNK_SIZE && y >= 0 && y < WORLD_HEIGHT && z >= 0 && z < CHUNK_SIZE;
}

glm::vec2 Chunk::GetClimate(int x, int z)
{
    return glm::vec2(m_terrain->temperaturemap[m_terrain->GetIndex(x, z)],
                     m_terrain->moisturemap[m_terrain->GetIndex(x, z)]);
}
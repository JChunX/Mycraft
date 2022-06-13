#include"Chunk.h"

Chunk::Chunk(int x, int z)
    : m_x(x),
      m_z(z),
      need_mesh_update(true),
      m_terrain(TerrainGenerator::GenerateTerrain(m_x, m_z))
{
    m_chunkdata = std::vector<Block>(CHUNK_SIZE * CHUNK_SIZE * WORLD_HEIGHT);
    Generate();
}

Chunk::Chunk()
    : Chunk(0, 0)
{

}

void Chunk::Generate()
{
    std::vector<BlockType> layers = BiomeManager::GetBlockLayers(m_terrain->biome_type);

    //auto res1 = std::async(&BiomeManager::GenerateBiomes, *this, 0, WORLD_HEIGHT, layers);
    //auto res2 = std::async(&BiomeManager::GenerateBiomes, *this, WORLD_HEIGHT/4, WORLD_HEIGHT/2, layers);
    //auto res3 = std::async(&BiomeManager::GenerateBiomes, *this, WORLD_HEIGHT/2, WORLD_HEIGHT*3/4, layers);
    //auto res4 = std::async(&BiomeManager::GenerateBiomes, *this, WORLD_HEIGHT*3/4, WORLD_HEIGHT, layers);
    BiomeManager::GenerateBiomes(*this, 0, WORLD_HEIGHT, layers);
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
    return x + y * CHUNK_SIZE * CHUNK_SIZE + z * CHUNK_SIZE;
}

bool Chunk::IsInChunk(int x, int y, int z)
{
    return x >= 0 && x < CHUNK_SIZE && y >= 0 && y < WORLD_HEIGHT && z >= 0 && z < CHUNK_SIZE;
}

Chunk::~Chunk()
{

}
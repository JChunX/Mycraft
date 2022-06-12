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
    auto res1 = std::async(&Chunk::GenerateAux, this, 0, WORLD_HEIGHT);
    auto res2 = std::async(&Chunk::GenerateAux, this, WORLD_HEIGHT/4, WORLD_HEIGHT/2);
    auto res3 = std::async(&Chunk::GenerateAux, this, WORLD_HEIGHT/2, WORLD_HEIGHT*3/4);
    auto res4 = std::async(&Chunk::GenerateAux, this, WORLD_HEIGHT*3/4, WORLD_HEIGHT);
}

void Chunk::GenerateAux(int ymin, int ymax)
{
    for (int y = ymin; y < ymax; y++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                float height = m_terrain.height[x][z];
                glm::vec3 block_position = glm::vec3(m_x+x,y,m_z+z);
                if (y == 0) 
                {
                    m_chunkdata[GetIndex(x, y, z)] = Block(BlockType::BEDROCK, block_position);
                }
                else if (y <= height - 2) {
                    m_chunkdata[GetIndex(x, y, z)] = Block(BlockType::STONE, block_position);
                }
                else if (y <= height - 1) {
                    m_chunkdata[GetIndex(x, y, z)] = Block(BlockType::DIRT, block_position);
                }
                else if (y <= height) {
                    m_chunkdata[GetIndex(x, y, z)] = Block(BlockType::GRASS, block_position);
                }
                else {
                    m_chunkdata[GetIndex(x, y, z)] = Block(BlockType::AIR, block_position);
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
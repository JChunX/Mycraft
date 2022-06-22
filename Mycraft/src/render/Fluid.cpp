#include "Fluid.h"


Fluid::Fluid(Scene& scene, std::pair<int, int> chunk_offset, TextureManager& texture_manager) : Mesh(scene, chunk_offset, texture_manager)
{

}

void Fluid::GenerateMesh()
{
    for (auto block : m_chunk.m_chunkdata)
    {
        if (block.block_type == BlockType::WATER)
        {
            std::vector<BlockFace> faces = DetermineVisibleFaces(block);
            for (auto face : faces)
            {
                AddFace(block, face);
            }
        }
    }
    std::unique_lock<std::mutex> lock(m_mutex);
    hasmesh = true;
}

std::vector<BlockFace> Fluid::DetermineVisibleFaces(Block& block)
{
    std::vector<BlockFace> faces;

    BlockType neighbour_type;

    int chunk_x = block.position.x - m_chunk.m_x;
    int chunk_y = block.position.y;
    int chunk_z = block.position.z - m_chunk.m_z;

    neighbour_type = m_chunk.GetBlockType(chunk_x, chunk_y + 1, chunk_z);
    if (neighbour_type == BlockType::AIR)
    {
        faces.push_back(BlockFace::TOP);
    }

    return faces;
}
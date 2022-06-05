#include "Mesh.h"

Mesh::Mesh(Scene& scene, std::pair<int, int> chunk_offset, TextureManager& texture_manager)
    : m_scene(scene),
      m_chunk(*(scene.GetChunk(chunk_offset))),
      m_texture_manager(texture_manager),
      m_vao(VAO()),
      m_vertices(std::vector<float>())
{
    n_faces = 0;
    GenerateMesh();
    Buffer();
}

Mesh::~Mesh()
{

}

void Mesh::Render()
{
    m_vao.Bind();
    glDrawArrays(GL_TRIANGLES, 0, n_faces*8);
}

void Mesh::Buffer()
{
    m_vao.Bind();
    float* vertices = &m_vertices[0];
    VBO vbo(vertices, n_faces*6*8*sizeof(float));
    m_vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 8*sizeof(float), (void*)0);
    m_vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 8*sizeof(float), (void*)(3*sizeof(float)));
    m_vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, 8*sizeof(float), (void*)(6*sizeof(float)));
    m_vao.Unbind();
    vbo.Unbind();
}

void Mesh::GenerateMesh()
{
    for (auto block : m_chunk.m_chunkdata)
    {
        if (block.block_type == BlockType::AIR 
         || block.block_type == BlockType::BEDROCK)
            continue;

        BlockType neighbour_type;

        int chunk_x = block.position.x - m_chunk.m_x;
        int chunk_y = block.position.y;
        int chunk_z = block.position.z - m_chunk.m_z;


        std::pair<int, int> neighbour_chunk_offset;
        if (chunk_x == 0) 
        {
            neighbour_chunk_offset = std::pair<int,int>(m_chunk.m_x-CHUNK_SIZE, m_chunk.m_z);
            Chunk* chunk =  m_scene.GetChunk(neighbour_chunk_offset);
            if (chunk)
            {
                neighbour_type = chunk->GetBlock(CHUNK_SIZE-1, chunk_y, chunk_z).block_type;
                if (neighbour_type == BlockType::AIR || neighbour_type == BlockType::NONE)
                {
                    AddFace(block, BlockFace::BACK);
                }
            }
        }
        else 
        {
            neighbour_type = m_chunk.GetBlock(chunk_x - 1, chunk_y, chunk_z).block_type;
            if (neighbour_type == BlockType::AIR)
            {
                AddFace(block, BlockFace::BACK);
            }
        }
        if (chunk_x == CHUNK_SIZE - 1)
        {
            neighbour_chunk_offset = std::pair<int,int>(m_chunk.m_x+CHUNK_SIZE, m_chunk.m_z);
            Chunk* chunk =  m_scene.GetChunk(neighbour_chunk_offset);
            if (chunk)
            {
                neighbour_type = chunk->GetBlock(0, chunk_y, chunk_z).block_type;
                if (neighbour_type == BlockType::AIR || neighbour_type == BlockType::NONE)
                {
                    AddFace(block, BlockFace::FRONT);
                }
            }
        }
        else
        {
            neighbour_type = m_chunk.GetBlock(chunk_x + 1, chunk_y, chunk_z).block_type;
            if (neighbour_type == BlockType::AIR)
            {
                AddFace(block, BlockFace::FRONT);
            }
        }
        if (chunk_z == 0)
        {
            neighbour_chunk_offset = std::pair<int,int>(m_chunk.m_x, m_chunk.m_z-CHUNK_SIZE);
            Chunk* chunk =  m_scene.GetChunk(neighbour_chunk_offset);
            if (chunk)
            {
                neighbour_type = chunk->GetBlock(chunk_x, chunk_y, CHUNK_SIZE-1).block_type;
                if (neighbour_type == BlockType::AIR || neighbour_type == BlockType::NONE)
                {
                    AddFace(block, BlockFace::LEFT);
                }
            }
        }
        else
        {
            neighbour_type = m_chunk.GetBlock(chunk_x, chunk_y, chunk_z - 1).block_type;
            if (neighbour_type == BlockType::AIR)
            {
                AddFace(block, BlockFace::LEFT);
            }
        }
        if (chunk_z == CHUNK_SIZE - 1)
        {
            neighbour_chunk_offset = std::pair<int,int>(m_chunk.m_x, m_chunk.m_z+CHUNK_SIZE);
            Chunk* chunk =  m_scene.GetChunk(neighbour_chunk_offset);
            if (chunk)
            {
                neighbour_type = chunk->GetBlock(chunk_x, chunk_y, 0).block_type;
                if (neighbour_type == BlockType::AIR || neighbour_type == BlockType::NONE)
                {
                    AddFace(block, BlockFace::RIGHT);
                }
            }
        }
        else
        {
            neighbour_type = m_chunk.GetBlock(chunk_x, chunk_y, chunk_z + 1).block_type;
            if (neighbour_type == BlockType::AIR)
            {
                AddFace(block, BlockFace::RIGHT);
            }
        }

        neighbour_type = m_chunk.GetBlock(chunk_x, chunk_y + 1, chunk_z).block_type;
        if (neighbour_type == BlockType::AIR)
        {
            AddFace(block, BlockFace::TOP);
        }

        neighbour_type = m_chunk.GetBlock(chunk_x, chunk_y - 1, chunk_z).block_type;
        if (neighbour_type == BlockType::AIR)
        {
            AddFace(block, BlockFace::BOTTOM);
        }
    }
}

void Mesh::AddFace(Block& block, BlockFace face)
{
    n_faces++;

    float x = block.position.x;
    float y = block.position.y;
    float z = block.position.z;

    glm::vec2 tex_coords = m_texture_manager.RetrieveTextureOffsets(block.block_type, face);

    float tex_coord_x = 2.0f/32.0f;
    float tex_coord_y = 31.0f/32.0f;

    //print out the texture coordinates
    //std::cout << tex_coord_x << " " << tex_coord_y << std::endl;

    switch (face)
    {
    case BlockFace::TOP:
        m_vertices.insert(m_vertices.end(), 
        {-0.5f+x,  0.5f+y, -0.5f+z,  0.0f,  1.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y,
          0.5f+x,  0.5f+y, -0.5f+z,  0.0f,  1.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y,
          0.5f+x,  0.5f+y,  0.5f+z,  0.0f,  1.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y,
          0.5f+x,  0.5f+y,  0.5f+z,  0.0f,  1.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y,
         -0.5f+x,  0.5f+y,  0.5f+z,  0.0f,  1.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y,
         -0.5f+x,  0.5f+y, -0.5f+z,  0.0f,  1.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y});
        break;
    case BlockFace::BOTTOM:
        m_vertices.insert(m_vertices.end(), 
        {-0.5f+x, -0.5f+y, -0.5f+z,  0.0f, -1.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y,
          0.5f+x, -0.5f+y, -0.5f+z,  0.0f, -1.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y,
          0.5f+x, -0.5f+y,  0.5f+z,  0.0f, -1.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y,
          0.5f+x, -0.5f+y,  0.5f+z,  0.0f, -1.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y,
         -0.5f+x, -0.5f+y,  0.5f+z,  0.0f, -1.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y,
         -0.5f+x, -0.5f+y, -0.5f+z,  0.0f, -1.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y});
        break;
    case BlockFace::FRONT:
        m_vertices.insert(m_vertices.end(), 
        {0.5f+x,  0.5f+y,  0.5f+z,  1.0f,  0.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y,
         0.5f+x,  0.5f+y, -0.5f+z,  1.0f,  0.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y,
         0.5f+x, -0.5f+y, -0.5f+z,  1.0f,  0.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y,
         0.5f+x, -0.5f+y, -0.5f+z,  1.0f,  0.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y,
         0.5f+x, -0.5f+y,  0.5f+z,  1.0f,  0.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y,
         0.5f+x,  0.5f+y,  0.5f+z,  1.0f,  0.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y});
        break;
    case BlockFace::BACK:
        m_vertices.insert(m_vertices.end(), 
        {-0.5f+x,  0.5f+y,  0.5f+z, -1.0f,  0.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y,
         -0.5f+x,  0.5f+y, -0.5f+z, -1.0f,  0.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y,
         -0.5f+x, -0.5f+y, -0.5f+z, -1.0f,  0.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y,
         -0.5f+x, -0.5f+y, -0.5f+z, -1.0f,  0.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y,
         -0.5f+x, -0.5f+y,  0.5f+z, -1.0f,  0.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y,
         -0.5f+x,  0.5f+y,  0.5f+z, -1.0f,  0.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y});
        break;
    case BlockFace::RIGHT:
        m_vertices.insert(m_vertices.end(), 
        {-0.5f+x, -0.5f+y, 0.5f+z,  0.0f,  0.0f,  1.0f,  0.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y,
          0.5f+x, -0.5f+y, 0.5f+z,  0.0f,  0.0f,  1.0f,  1.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y,
          0.5f+x,  0.5f+y, 0.5f+z,  0.0f,  0.0f,  1.0f,  1.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y,
          0.5f+x,  0.5f+y, 0.5f+z,  0.0f,  0.0f,  1.0f,  1.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y,
         -0.5f+x,  0.5f+y, 0.5f+z,  0.0f,  0.0f,  1.0f,  0.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y,
         -0.5f+x, -0.5f+y, 0.5f+z,  0.0f,  0.0f,  1.0f,  0.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y});
        break;
    case BlockFace::LEFT:
        m_vertices.insert(m_vertices.end(), 
        {-0.5f+x, -0.5f+y, -0.5f+z,  0.0f,  0.0f, -1.0f,  0.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y,
          0.5f+x, -0.5f+y, -0.5f+z,  0.0f,  0.0f, -1.0f,  1.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y,
          0.5f+x,  0.5f+y, -0.5f+z,  0.0f,  0.0f, -1.0f,  1.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y,
          0.5f+x,  0.5f+y, -0.5f+z,  0.0f,  0.0f, -1.0f,  1.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y,
         -0.5f+x,  0.5f+y, -0.5f+z,  0.0f,  0.0f, -1.0f,  0.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y,
         -0.5f+x, -0.5f+y, -0.5f+z,  0.0f,  0.0f, -1.0f,  0.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y});
        break;
    }
}
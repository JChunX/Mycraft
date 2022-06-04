#include "Mesh.h"

Mesh::Mesh(Chunk& chunk)
    : m_chunk(chunk),
      m_vao(VAO()),
      m_vertices(std::vector<float>()),
      m_texture_manager(TextureManager())
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
    m_texture_manager.BindTexture();
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
        if (block.block_type == BlockType::AIR)
            continue;

        BlockType neighbour_type;

        int chunk_x = block.position.x - m_chunk.m_x;
        int chunk_y = block.position.y;
        int chunk_z = block.position.z - m_chunk.m_z;

        neighbour_type = m_chunk.GetBlock(chunk_x, chunk_y + 1, chunk_z).block_type;
        if (neighbour_type == BlockType::AIR || neighbour_type == BlockType::NONE)
        {
            AddFace(block, BlockFace::TOP);
        }

        neighbour_type = m_chunk.GetBlock(chunk_x, chunk_y - 1, chunk_z).block_type;
        if (neighbour_type == BlockType::AIR || neighbour_type == BlockType::NONE)
        {
            AddFace(block, BlockFace::BOTTOM);
        }

        neighbour_type = m_chunk.GetBlock(chunk_x, chunk_y, chunk_z + 1).block_type;
        if (neighbour_type == BlockType::AIR || neighbour_type == BlockType::NONE)
        {
            AddFace(block, BlockFace::RIGHT);
        }

        neighbour_type = m_chunk.GetBlock(chunk_x, chunk_y, chunk_z - 1).block_type;
        if (neighbour_type == BlockType::AIR || neighbour_type == BlockType::NONE)
        {
            AddFace(block, BlockFace::LEFT);
        }

        neighbour_type = m_chunk.GetBlock(chunk_x + 1, chunk_y, chunk_z).block_type;
        if (neighbour_type == BlockType::AIR || neighbour_type == BlockType::NONE)
        {
            AddFace(block, BlockFace::FRONT);
        }

        neighbour_type = m_chunk.GetBlock(chunk_x - 1, chunk_y, chunk_z).block_type;
        if (neighbour_type == BlockType::AIR || neighbour_type == BlockType::NONE)
        {
            AddFace(block, BlockFace::BACK);
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

    float tex_coord_x = 0.0f;
    float tex_coord_y = 15.0f/16.0f;

    //print out the texture coordinates
    //std::cout << tex_coord_x << " " << tex_coord_y << std::endl;

    switch (face)
    {
    case BlockFace::TOP:
        m_vertices.insert(m_vertices.end(), 
        {-0.5f+x,  0.5f+y, -0.5f+z,  0.0f,  1.0f,  0.0f,  0.0f/16.0f+tex_coord_x,  1.0f/16.0f+tex_coord_y,
          0.5f+x,  0.5f+y, -0.5f+z,  0.0f,  1.0f,  0.0f,  1.0f/16.0f+tex_coord_x,  1.0f/16.0f+tex_coord_y,
          0.5f+x,  0.5f+y,  0.5f+z,  0.0f,  1.0f,  0.0f,  1.0f/16.0f+tex_coord_x,  0.0f/16.0f+tex_coord_y,
          0.5f+x,  0.5f+y,  0.5f+z,  0.0f,  1.0f,  0.0f,  1.0f/16.0f+tex_coord_x,  0.0f/16.0f+tex_coord_y,
         -0.5f+x,  0.5f+y,  0.5f+z,  0.0f,  1.0f,  0.0f,  0.0f/16.0f+tex_coord_x,  0.0f/16.0f+tex_coord_y,
         -0.5f+x,  0.5f+y, -0.5f+z,  0.0f,  1.0f,  0.0f,  0.0f/16.0f+tex_coord_x,  1.0f/16.0f+tex_coord_y});
        break;
    case BlockFace::BOTTOM:
        m_vertices.insert(m_vertices.end(), 
        {-0.5f+x, -0.5f+y, -0.5f+z,  0.0f, -1.0f,  0.0f,  0.0f/16.0f+tex_coord_x,  1.0f/16.0f+tex_coord_y,
          0.5f+x, -0.5f+y, -0.5f+z,  0.0f, -1.0f,  0.0f,  1.0f/16.0f+tex_coord_x,  1.0f/16.0f+tex_coord_y,
          0.5f+x, -0.5f+y,  0.5f+z,  0.0f, -1.0f,  0.0f,  1.0f/16.0f+tex_coord_x,  0.0f/16.0f+tex_coord_y,
          0.5f+x, -0.5f+y,  0.5f+z,  0.0f, -1.0f,  0.0f,  1.0f/16.0f+tex_coord_x,  0.0f/16.0f+tex_coord_y,
         -0.5f+x, -0.5f+y,  0.5f+z,  0.0f, -1.0f,  0.0f,  0.0f/16.0f+tex_coord_x,  0.0f/16.0f+tex_coord_y,
         -0.5f+x, -0.5f+y, -0.5f+z,  0.0f, -1.0f,  0.0f,  0.0f/16.0f+tex_coord_x,  1.0f/16.0f+tex_coord_y});
        break;
    case BlockFace::FRONT:
        m_vertices.insert(m_vertices.end(), 
        {0.5f+x,  0.5f+y,  0.5f+z,  1.0f,  0.0f,  0.0f,  1.0f/16.0f+tex_coord_x,  0.0f/16.0f+tex_coord_y,
         0.5f+x,  0.5f+y, -0.5f+z,  1.0f,  0.0f,  0.0f,  1.0f/16.0f+tex_coord_x,  1.0f/16.0f+tex_coord_y,
         0.5f+x, -0.5f+y, -0.5f+z,  1.0f,  0.0f,  0.0f,  0.0f/16.0f+tex_coord_x,  1.0f/16.0f+tex_coord_y,
         0.5f+x, -0.5f+y, -0.5f+z,  1.0f,  0.0f,  0.0f,  0.0f/16.0f+tex_coord_x,  1.0f/16.0f+tex_coord_y,
         0.5f+x, -0.5f+y,  0.5f+z,  1.0f,  0.0f,  0.0f,  0.0f/16.0f+tex_coord_x,  0.0f/16.0f+tex_coord_y,
         0.5f+x,  0.5f+y,  0.5f+z,  1.0f,  0.0f,  0.0f,  1.0f/16.0f+tex_coord_x,  0.0f/16.0f+tex_coord_y});
        break;
    case BlockFace::BACK:
        m_vertices.insert(m_vertices.end(), 
        {-0.5f+x,  0.5f+y,  0.5f+z, -1.0f,  0.0f,  0.0f,  1.0f/16.0f+tex_coord_x,  0.0f/16.0f+tex_coord_y,
         -0.5f+x,  0.5f+y, -0.5f+z, -1.0f,  0.0f,  0.0f,  1.0f/16.0f+tex_coord_x,  1.0f/16.0f+tex_coord_y,
         -0.5f+x, -0.5f+y, -0.5f+z, -1.0f,  0.0f,  0.0f,  0.0f/16.0f+tex_coord_x,  1.0f/16.0f+tex_coord_y,
         -0.5f+x, -0.5f+y, -0.5f+z, -1.0f,  0.0f,  0.0f,  0.0f/16.0f+tex_coord_x,  1.0f/16.0f+tex_coord_y,
         -0.5f+x, -0.5f+y,  0.5f+z, -1.0f,  0.0f,  0.0f,  0.0f/16.0f+tex_coord_x,  0.0f/16.0f+tex_coord_y,
         -0.5f+x,  0.5f+y,  0.5f+z, -1.0f,  0.0f,  0.0f,  1.0f/16.0f+tex_coord_x,  0.0f/16.0f+tex_coord_y});
        break;
    case BlockFace::RIGHT:
        m_vertices.insert(m_vertices.end(), 
        {-0.5f+x, -0.5f+y, 0.5f+z,  0.0f,  0.0f,  1.0f,  0.0f/16.0f+tex_coord_x,  0.0f/16.0f+tex_coord_y,
          0.5f+x, -0.5f+y, 0.5f+z,  0.0f,  0.0f,  1.0f,  1.0f/16.0f+tex_coord_x,  0.0f/16.0f+tex_coord_y,
          0.5f+x,  0.5f+y, 0.5f+z,  0.0f,  0.0f,  1.0f,  1.0f/16.0f+tex_coord_x,  1.0f/16.0f+tex_coord_y,
          0.5f+x,  0.5f+y, 0.5f+z,  0.0f,  0.0f,  1.0f,  1.0f/16.0f+tex_coord_x,  1.0f/16.0f+tex_coord_y,
         -0.5f+x,  0.5f+y, 0.5f+z,  0.0f,  0.0f,  1.0f,  0.0f/16.0f+tex_coord_x,  1.0f/16.0f+tex_coord_y,
         -0.5f+x, -0.5f+y, 0.5f+z,  0.0f,  0.0f,  1.0f,  0.0f/16.0f+tex_coord_x,  0.0f/16.0f+tex_coord_y});
        break;
    case BlockFace::LEFT:
        m_vertices.insert(m_vertices.end(), 
        {-0.5f+x, -0.5f+y, -0.5f+z,  0.0f,  0.0f, -1.0f,  0.0f/16.0f+tex_coord_x,  0.0f/16.0f+tex_coord_y,
          0.5f+x, -0.5f+y, -0.5f+z,  0.0f,  0.0f, -1.0f,  1.0f/16.0f+tex_coord_x,  0.0f/16.0f+tex_coord_y,
          0.5f+x,  0.5f+y, -0.5f+z,  0.0f,  0.0f, -1.0f,  1.0f/16.0f+tex_coord_x,  1.0f/16.0f+tex_coord_y,
          0.5f+x,  0.5f+y, -0.5f+z,  0.0f,  0.0f, -1.0f,  1.0f/16.0f+tex_coord_x,  1.0f/16.0f+tex_coord_y,
         -0.5f+x,  0.5f+y, -0.5f+z,  0.0f,  0.0f, -1.0f,  0.0f/16.0f+tex_coord_x,  1.0f/16.0f+tex_coord_y,
         -0.5f+x, -0.5f+y, -0.5f+z,  0.0f,  0.0f, -1.0f,  0.0f/16.0f+tex_coord_x,  0.0f/16.0f+tex_coord_y});
        break;
    }
}
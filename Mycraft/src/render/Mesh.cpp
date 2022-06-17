#include "Mesh.h"
#include "Helpers.h"

Mesh::Mesh(Scene& scene, std::pair<int, int> chunk_offset, TextureManager& texture_manager, float temperature, float moisture)
    : m_scene(scene),
      m_chunk(*(scene.GetChunk(chunk_offset))),
      m_texture_manager(texture_manager),
      m_vao(VAO()),
      m_vbo(VBO()),
      m_vertices(std::vector<float>()),
      should_erase(false),
      n_faces(0),
      m_temperature(temperature),
      m_moisture(moisture)
{
    GenerateMesh();
    Buffer();
}

Mesh::Mesh(Scene& scene, std::pair<int, int> chunk_offset, TextureManager& texture_manager)
    : Mesh(scene, chunk_offset, texture_manager, 16.0f, 16.0f)
{

}

Mesh::~Mesh()
{
    Delete();
}

void Mesh::Render()
{
    m_vao.Bind();
    glDrawArrays(GL_TRIANGLES, 0, n_faces*12);
}

void Mesh::Delete()
{
    m_vbo.Delete();
    m_vao.Delete();
}

void Mesh::Buffer()
{
    m_vao.Bind();
    float* vertices = &m_vertices[0];
    m_vbo.Buffer(vertices, n_faces*6*12*sizeof(float)); 
    m_vao.LinkAttrib(m_vbo, 0, 3, GL_FLOAT, 12*sizeof(float), (void*)0);
    m_vao.LinkAttrib(m_vbo, 1, 3, GL_FLOAT, 12*sizeof(float), (void*)(3*sizeof(float)));
    m_vao.LinkAttrib(m_vbo, 2, 2, GL_FLOAT, 12*sizeof(float), (void*)(6*sizeof(float)));
    m_vao.LinkAttrib(m_vbo, 3, 4, GL_FLOAT, 12*sizeof(float), (void*)(8*sizeof(float)));
    m_vao.Unbind();
    m_vbo.Unbind();
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
                neighbour_type = chunk->GetBlockType(CHUNK_SIZE-1, chunk_y, chunk_z);
                if (neighbour_type == BlockType::AIR || neighbour_type == BlockType::NONE)
                {
                    AddFace(block, BlockFace::BACK);
                }
            }
        }
        else 
        {
            neighbour_type = m_chunk.GetBlockType(chunk_x - 1, chunk_y, chunk_z);
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
                neighbour_type = chunk->GetBlockType(0, chunk_y, chunk_z);
                if (neighbour_type == BlockType::AIR || neighbour_type == BlockType::NONE)
                {
                    AddFace(block, BlockFace::FRONT);
                }
            }
        }
        else
        {
            neighbour_type = m_chunk.GetBlockType(chunk_x + 1, chunk_y, chunk_z);
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
                neighbour_type = chunk->GetBlockType(chunk_x, chunk_y, CHUNK_SIZE-1);
                if (neighbour_type == BlockType::AIR || neighbour_type == BlockType::NONE)
                {
                    AddFace(block, BlockFace::LEFT);
                }
            }
        }
        else
        {
            neighbour_type = m_chunk.GetBlockType(chunk_x, chunk_y, chunk_z - 1);
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
                neighbour_type = chunk->GetBlockType(chunk_x, chunk_y, 0);
                if (neighbour_type == BlockType::AIR || neighbour_type == BlockType::NONE)
                {
                    AddFace(block, BlockFace::RIGHT);
                }
            }
        }
        else
        {
            neighbour_type = m_chunk.GetBlockType(chunk_x, chunk_y, chunk_z + 1);
            if (neighbour_type == BlockType::AIR)
            {
                AddFace(block, BlockFace::RIGHT);
            }
        }

        neighbour_type = m_chunk.GetBlockType(chunk_x, chunk_y + 1, chunk_z);
        if (neighbour_type == BlockType::AIR)
        {
            AddFace(block, BlockFace::TOP);
        }

        neighbour_type = m_chunk.GetBlockType(chunk_x, chunk_y - 1, chunk_z);
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

    std::pair<float,float> tex_coords = m_texture_manager.RetrieveTextureOffsets(block.block_type, face);

    float tex_coord_x = tex_coords.first/32.0f;
    float tex_coord_y = tex_coords.second/32.0f;

    glm::vec2 climate = m_chunk.GetClimate(block.position.x - m_chunk.m_x,
                                               block.position.z - m_chunk.m_z);

    glm::vec4 recolor = m_texture_manager.RetrieveTextureRecolor(block.block_type, face, climate.x, climate.y);

    switch (face)
    {
    case BlockFace::TOP:
        m_vertices.insert(m_vertices.end(), 
        {-0.5f+x,  0.5f+y, -0.5f+z,  0.0f,  1.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
          0.5f+x,  0.5f+y, -0.5f+z,  0.0f,  1.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
          0.5f+x,  0.5f+y,  0.5f+z,  0.0f,  1.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
          0.5f+x,  0.5f+y,  0.5f+z,  0.0f,  1.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         -0.5f+x,  0.5f+y,  0.5f+z,  0.0f,  1.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         -0.5f+x,  0.5f+y, -0.5f+z,  0.0f,  1.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,});
        break;
    case BlockFace::BOTTOM:
        m_vertices.insert(m_vertices.end(), 
        {-0.5f+x, -0.5f+y, -0.5f+z,  0.0f, -1.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
          0.5f+x, -0.5f+y, -0.5f+z,  0.0f, -1.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
          0.5f+x, -0.5f+y,  0.5f+z,  0.0f, -1.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
          0.5f+x, -0.5f+y,  0.5f+z,  0.0f, -1.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         -0.5f+x, -0.5f+y,  0.5f+z,  0.0f, -1.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         -0.5f+x, -0.5f+y, -0.5f+z,  0.0f, -1.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,});
        break;
    case BlockFace::RIGHT:
        m_vertices.insert(m_vertices.end(), 
        {-0.5f+x, -0.5f+y, 0.5f+z,  0.0f,  0.0f,  1.0f,  0.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
          0.5f+x, -0.5f+y, 0.5f+z,  0.0f,  0.0f,  1.0f,  1.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
          0.5f+x,  0.5f+y, 0.5f+z,  0.0f,  0.0f,  1.0f,  1.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
          0.5f+x,  0.5f+y, 0.5f+z,  0.0f,  0.0f,  1.0f,  1.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         -0.5f+x,  0.5f+y, 0.5f+z,  0.0f,  0.0f,  1.0f,  0.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         -0.5f+x, -0.5f+y, 0.5f+z,  0.0f,  0.0f,  1.0f,  0.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,});
        break;
    case BlockFace::LEFT:
        m_vertices.insert(m_vertices.end(), 
        {-0.5f+x, -0.5f+y, -0.5f+z,  0.0f,  0.0f, -1.0f,  0.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
          0.5f+x, -0.5f+y, -0.5f+z,  0.0f,  0.0f, -1.0f,  1.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
          0.5f+x,  0.5f+y, -0.5f+z,  0.0f,  0.0f, -1.0f,  1.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
          0.5f+x,  0.5f+y, -0.5f+z,  0.0f,  0.0f, -1.0f,  1.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         -0.5f+x,  0.5f+y, -0.5f+z,  0.0f,  0.0f, -1.0f,  0.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         -0.5f+x, -0.5f+y, -0.5f+z,  0.0f,  0.0f, -1.0f,  0.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,});
        break;
    case BlockFace::FRONT:
        m_vertices.insert(m_vertices.end(), 
        {0.5f+x,  0.5f+y,  0.5f+z,  1.0f,  0.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         0.5f+x,  0.5f+y, -0.5f+z,  1.0f,  0.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         0.5f+x, -0.5f+y, -0.5f+z,  1.0f,  0.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         0.5f+x, -0.5f+y, -0.5f+z,  1.0f,  0.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         0.5f+x, -0.5f+y,  0.5f+z,  1.0f,  0.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         0.5f+x,  0.5f+y,  0.5f+z,  1.0f,  0.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,});
        break;
    case BlockFace::BACK:
        m_vertices.insert(m_vertices.end(), 
        {-0.5f+x,  0.5f+y,  0.5f+z, -1.0f,  0.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         -0.5f+x,  0.5f+y, -0.5f+z, -1.0f,  0.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         -0.5f+x, -0.5f+y, -0.5f+z, -1.0f,  0.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         -0.5f+x, -0.5f+y, -0.5f+z, -1.0f,  0.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         -0.5f+x, -0.5f+y,  0.5f+z, -1.0f,  0.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         -0.5f+x,  0.5f+y,  0.5f+z, -1.0f,  0.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,});
        break;
    }
}
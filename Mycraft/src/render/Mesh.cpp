#include "Mesh.h"
#include "Helpers.h"

Mesh::Mesh(Scene& scene, std::pair<int, int> chunk_offset, TextureManager& texture_manager)
    : m_scene(scene),
      m_chunk(*(scene.GetChunk(chunk_offset))),
      Renderable(texture_manager)
{

}

Mesh::~Mesh()
{
    Delete();
}

void Mesh::Render()
{
    //std::unique_lock<std::mutex> lock(m_mutex);
    m_vao.Bind();
    m_texture_manager.BindTexture(*(m_texture_manager.m_block_texture));
    glDrawArrays(GL_TRIANGLES, 0, n_faces*12);
    m_vao.Unbind();
}

void Mesh::Buffer()
{
    //std::unique_lock<std::mutex> lock(m_mutex);
    m_vao.Bind();
    float* vertices = &m_vertices[0];
    m_vbo.Buffer(vertices, n_faces*6*12*sizeof(float)); 
    m_vao.LinkAttrib(m_vbo, 0, 3, GL_FLOAT, 12*sizeof(float), (void*)0);
    m_vao.LinkAttrib(m_vbo, 1, 3, GL_FLOAT, 12*sizeof(float), (void*)(3*sizeof(float)));
    m_vao.LinkAttrib(m_vbo, 2, 2, GL_FLOAT, 12*sizeof(float), (void*)(6*sizeof(float)));
    m_vao.LinkAttrib(m_vbo, 3, 4, GL_FLOAT, 12*sizeof(float), (void*)(8*sizeof(float)));
    m_vao.Unbind();
    m_vbo.Unbind();
    buffered = true;
}

void Mesh::UpdateBuffer()
{
    //std::unique_lock<std::mutex> lock(m_mutex);
    if(buffered)
    {
        float* vertices = &m_vertices[0];
        std::cout << "Updating buffer, nfaces= " << n_faces << std::endl;
        m_vbo.Buffer(vertices, n_faces*6*12*sizeof(float)); 
        m_vbo.Unbind();
    }
}

void Mesh::GenerateMesh()
{
    m_vertices.clear();
    //boost::asio::thread_pool thread_pool(NUM_RENDER_POOL_WORKERS);
    n_faces = 0;
    for (auto block : m_chunk.m_chunkdata)
    {    
        if (block.block_type == BlockType::AIR
            || block.block_type == BlockType::BEDROCK
            || block.block_type == BlockType::WATER)
            continue;
        //boost::asio::post(thread_pool, std::bind(&Mesh::GenerateMeshAux, this, block));
        GenerateMeshAux(block);
        
    }
    //thread_pool.join();
    hasmesh = true;
}

void Mesh::GenerateMeshAux(Block& block)
{
    std::vector<BlockFace> faces = DetermineVisibleFaces(block);
    for (auto face : faces)
    {
        AddFace(block, face);
    }
}

std::vector<BlockFace> Mesh::DetermineVisibleFaces(Block& block)
{

    std::vector<BlockFace> faces;
    BlockType neighbour_type;

    int chunk_x = block.position.x - m_chunk.m_x;
    int chunk_y = block.position.y;
    int chunk_z = block.position.z - m_chunk.m_z;

    std::pair<int, int> neighbour_chunk_offset;
    if (chunk_x == 0) 
    {
        neighbour_chunk_offset = std::pair<int,int>(m_chunk.m_x-CHUNK_SIZE, m_chunk.m_z);
        auto chunk =  m_scene.GetChunk(neighbour_chunk_offset);
        if (chunk)
        {
            neighbour_type = chunk->GetBlockType(CHUNK_SIZE-1, chunk_y, chunk_z);
            if (neighbour_type == BlockType::AIR || neighbour_type == BlockType::WATER || neighbour_type == BlockType::NONE)
            {
                faces.push_back(BlockFace::BACK);
            }
        }
    }
    else 
    {
        neighbour_type = m_chunk.GetBlockType(chunk_x - 1, chunk_y, chunk_z);
        if (neighbour_type == BlockType::AIR || neighbour_type == BlockType::WATER)
        {
            faces.push_back(BlockFace::BACK);
        }
    }
    if (chunk_x == CHUNK_SIZE - 1)
    {
        neighbour_chunk_offset = std::pair<int,int>(m_chunk.m_x+CHUNK_SIZE, m_chunk.m_z);
        auto chunk =  m_scene.GetChunk(neighbour_chunk_offset);
        if (chunk)
        {
            neighbour_type = chunk->GetBlockType(0, chunk_y, chunk_z);
            if (neighbour_type == BlockType::AIR || neighbour_type == BlockType::WATER || neighbour_type == BlockType::NONE)
            {
                faces.push_back(BlockFace::FRONT);
            }
        }
    }
    else
    {
        neighbour_type = m_chunk.GetBlockType(chunk_x + 1, chunk_y, chunk_z);
        if (neighbour_type == BlockType::AIR || neighbour_type == BlockType::WATER)
        {
            faces.push_back(BlockFace::FRONT);
        }
    }
    if (chunk_z == 0)
    {
        neighbour_chunk_offset = std::pair<int,int>(m_chunk.m_x, m_chunk.m_z-CHUNK_SIZE);
        auto chunk =  m_scene.GetChunk(neighbour_chunk_offset);
        if (chunk)
        {
            neighbour_type = chunk->GetBlockType(chunk_x, chunk_y, CHUNK_SIZE-1);
            if (neighbour_type == BlockType::AIR || neighbour_type == BlockType::WATER || neighbour_type == BlockType::NONE)
            {
                faces.push_back(BlockFace::LEFT);
            }
        }
    }
    else
    {
        neighbour_type = m_chunk.GetBlockType(chunk_x, chunk_y, chunk_z - 1);
        if (neighbour_type == BlockType::AIR || neighbour_type == BlockType::WATER)
        {
            faces.push_back(BlockFace::LEFT);
        }
    }
    if (chunk_z == CHUNK_SIZE - 1)
    {
        neighbour_chunk_offset = std::pair<int,int>(m_chunk.m_x, m_chunk.m_z+CHUNK_SIZE);
        auto chunk =  m_scene.GetChunk(neighbour_chunk_offset);
        if (chunk)
        {
            neighbour_type = chunk->GetBlockType(chunk_x, chunk_y, 0);
            if (neighbour_type == BlockType::AIR || neighbour_type == BlockType::WATER || neighbour_type == BlockType::NONE)
            {
                faces.push_back(BlockFace::RIGHT);
            }
        }
    }
    else
    {
        neighbour_type = m_chunk.GetBlockType(chunk_x, chunk_y, chunk_z + 1);
        if (neighbour_type == BlockType::AIR || neighbour_type == BlockType::WATER)
        {
            faces.push_back(BlockFace::RIGHT);
        }
    }

    neighbour_type = m_chunk.GetBlockType(chunk_x, chunk_y + 1, chunk_z);
    if (neighbour_type == BlockType::AIR || neighbour_type == BlockType::WATER)
    {
        faces.push_back(BlockFace::TOP);
    }

    neighbour_type = m_chunk.GetBlockType(chunk_x, chunk_y - 1, chunk_z);
    if (neighbour_type == BlockType::AIR || neighbour_type == BlockType::WATER)
    {
        faces.push_back(BlockFace::BOTTOM);
    }

    return faces;
}

void Mesh::AddFace(Block& block, BlockFace face)
{
    //std::unique_lock<std::mutex> blockface_lock(m_mutex);
    n_faces++;
    //blockface_lock.unlock();
    float x = block.position.x;
    float y = block.position.y;
    float z = block.position.z;

    std::pair<float,float> tex_coords = m_texture_manager.RetrieveBlockTextureOffsets(block.block_type, face);

    float tex_coord_x = tex_coords.first/32.0f;
    float tex_coord_y = tex_coords.second/32.0f;

    glm::vec2 climate = m_chunk.GetClimate(block.position.x - m_chunk.m_x,
                                               block.position.z - m_chunk.m_z);

    glm::vec4 recolor = m_texture_manager.RetrieveBlockTextureRecolor(block.block_type, face, climate.x, climate.y);

    //std::unique_lock<std::mutex> vertices_lock(m_mutex);
    std::vector<float> new_vertices;
    switch (face)
    {
    case BlockFace::TOP:
         new_vertices = {
         -0.5f+x,  0.5f+y, -0.5f+z,  0.0f,  1.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
          0.5f+x,  0.5f+y, -0.5f+z,  0.0f,  1.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
          0.5f+x,  0.5f+y,  0.5f+z,  0.0f,  1.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
          0.5f+x,  0.5f+y,  0.5f+z,  0.0f,  1.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         -0.5f+x,  0.5f+y,  0.5f+z,  0.0f,  1.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         -0.5f+x,  0.5f+y, -0.5f+z,  0.0f,  1.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w};
        break;
    case BlockFace::BOTTOM:
        new_vertices = {
         -0.5f+x, -0.5f+y, -0.5f+z,  0.0f, -1.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
          0.5f+x, -0.5f+y, -0.5f+z,  0.0f, -1.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
          0.5f+x, -0.5f+y,  0.5f+z,  0.0f, -1.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
          0.5f+x, -0.5f+y,  0.5f+z,  0.0f, -1.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         -0.5f+x, -0.5f+y,  0.5f+z,  0.0f, -1.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         -0.5f+x, -0.5f+y, -0.5f+z,  0.0f, -1.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w};
        break;
    case BlockFace::RIGHT:
        new_vertices = {
         -0.5f+x, -0.5f+y, 0.5f+z,  0.0f,  0.0f,  1.0f,  0.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
          0.5f+x, -0.5f+y, 0.5f+z,  0.0f,  0.0f,  1.0f,  1.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
          0.5f+x,  0.5f+y, 0.5f+z,  0.0f,  0.0f,  1.0f,  1.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
          0.5f+x,  0.5f+y, 0.5f+z,  0.0f,  0.0f,  1.0f,  1.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         -0.5f+x,  0.5f+y, 0.5f+z,  0.0f,  0.0f,  1.0f,  0.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         -0.5f+x, -0.5f+y, 0.5f+z,  0.0f,  0.0f,  1.0f,  0.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w};
        break;
    case BlockFace::LEFT:
        new_vertices = {
         -0.5f+x, -0.5f+y, -0.5f+z,  0.0f,  0.0f, -1.0f,  0.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
          0.5f+x, -0.5f+y, -0.5f+z,  0.0f,  0.0f, -1.0f,  1.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
          0.5f+x,  0.5f+y, -0.5f+z,  0.0f,  0.0f, -1.0f,  1.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
          0.5f+x,  0.5f+y, -0.5f+z,  0.0f,  0.0f, -1.0f,  1.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         -0.5f+x,  0.5f+y, -0.5f+z,  0.0f,  0.0f, -1.0f,  0.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         -0.5f+x, -0.5f+y, -0.5f+z,  0.0f,  0.0f, -1.0f,  0.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w};
        break;
    case BlockFace::FRONT:
        new_vertices = {
         0.5f+x,  0.5f+y,  0.5f+z,  1.0f,  0.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         0.5f+x,  0.5f+y, -0.5f+z,  1.0f,  0.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         0.5f+x, -0.5f+y, -0.5f+z,  1.0f,  0.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         0.5f+x, -0.5f+y, -0.5f+z,  1.0f,  0.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         0.5f+x, -0.5f+y,  0.5f+z,  1.0f,  0.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         0.5f+x,  0.5f+y,  0.5f+z,  1.0f,  0.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w};
        break;
    case BlockFace::BACK:
        new_vertices = {
         -0.5f+x,  0.5f+y,  0.5f+z, -1.0f,  0.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         -0.5f+x,  0.5f+y, -0.5f+z, -1.0f,  0.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         -0.5f+x, -0.5f+y, -0.5f+z, -1.0f,  0.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         -0.5f+x, -0.5f+y, -0.5f+z, -1.0f,  0.0f,  0.0f,  1.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         -0.5f+x, -0.5f+y,  0.5f+z, -1.0f,  0.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  0.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w,
         -0.5f+x,  0.5f+y,  0.5f+z, -1.0f,  0.0f,  0.0f,  0.0f/32.0f+tex_coord_x,  1.0f/32.0f+tex_coord_y, recolor.x, recolor.y, recolor.z, recolor.w};
        break;
    }
    m_vertices.insert(m_vertices.end(), new_vertices.begin(), new_vertices.end());
}
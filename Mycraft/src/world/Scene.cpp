#include"Scene.h"

Scene::Scene(Camera& camera)
    : m_camera(camera)
{

}

void Scene::Begin(bool* terminate_flag)
{
    float last_update = glfwGetTime();
    float delta;
    while (!(*terminate_flag))
    {
        // only update if enough time has passed
        delta = glfwGetTime() - last_update;
        if (delta > 1.0f/SCENE_UPDATE_FREQ)
        {
            Update();
            last_update = glfwGetTime();
        }

    }
}

void Scene::Update()
{
    glm::vec3 position = m_camera.position;

    LoadChunks(position.x, position.z);

    //print camera heading
    glm::vec3 heading = m_camera.GetHeadingVector();
    Block* current_block = GetRaycastTarget(position, heading);

    if (current_block != nullptr)
    {
        std::cout << "Current block position: " << current_block->position.x << " " << current_block->position.y << " " << current_block->position.z << std::endl;
    }
}

void Scene::LoadChunks(int x, int z)
{
    
    glm::vec2 nearest_chunk_offset = static_cast<float>(CHUNK_SIZE) * glm::vec2(round(x / CHUNK_SIZE), round(z / CHUNK_SIZE));
    int xc0 = nearest_chunk_offset.x;
    int zc0 = nearest_chunk_offset.y;

    int min_xc = xc0 - CHUNK_LOAD_DISTANCE * CHUNK_SIZE;
    int min_zc = zc0 - CHUNK_LOAD_DISTANCE * CHUNK_SIZE;
    int max_xc = xc0 + (CHUNK_LOAD_DISTANCE-1) * CHUNK_SIZE;
    int max_zc = zc0 + (CHUNK_LOAD_DISTANCE-1) * CHUNK_SIZE;

    // remove far chunks
    for (auto it = m_current_chunks.begin(), next_it = it; it != m_current_chunks.end(); it = next_it)
    {
        ++next_it;
        if (!ShouldRender(min_xc, min_zc, max_xc, max_zc, it->second))
        {
            std::unique_lock<std::mutex> chunk_lock(m_chunks_mutex);
            std::unique_lock<std::mutex> mesh_lock(m_meshes_mutex);

            m_meshes.erase(it->first);(m_chunks_mutex);
            m_current_chunks.erase(it);
            
            chunk_lock.unlock();
            mesh_lock.unlock();
            std::cout << "erase" << std::endl;
        }
    }

    // load chunks
    for (int chunk_idx_x=-CHUNK_LOAD_DISTANCE; chunk_idx_x<CHUNK_LOAD_DISTANCE; chunk_idx_x++)
    {
        for (int chunk_idx_z=-CHUNK_LOAD_DISTANCE; chunk_idx_z<CHUNK_LOAD_DISTANCE; chunk_idx_z++)
        {
            int xc = xc0 + chunk_idx_x * CHUNK_SIZE;
            int zc = zc0 + chunk_idx_z * CHUNK_SIZE;
            std::pair<int,int> key = std::pair<int, int>(xc, zc);
            // if not in occupancy, load chunk
            if (m_current_chunks.find(key) == m_current_chunks.end())
            {
                auto res = std::async(&Scene::LoadChunkAux, this, key);
            }
        }
    }
}

void Scene::LoadChunkAux(std::pair<int,int> chunk_coords)
{
    int xc = chunk_coords.first;
    int zc = chunk_coords.second;
    Chunk chunk(xc, zc);
    m_current_chunks.insert({chunk_coords, chunk});
    //print load 
    std::cout << "load chunk: " << xc << " " << zc << std::endl;
    // neighbor chunks now need update
    for (int neighbor_idx_x=-1; neighbor_idx_x<=1; neighbor_idx_x++)
    {
        for (int neighbor_idx_z=-1; neighbor_idx_z<=1; neighbor_idx_z++)
        {
            if ((neighbor_idx_x == 0 && neighbor_idx_z == 0) || (neighbor_idx_x != 0 && neighbor_idx_z != 0))
            {
                continue;
            }
            int neighbor_xc = xc + neighbor_idx_x * CHUNK_SIZE;
            int neighbor_zc = zc + neighbor_idx_z * CHUNK_SIZE;
            std::pair<int,int> neighbor_key = std::pair<int, int>(neighbor_xc, neighbor_zc);
            //std::unique_lock<std::mutex> lock(m_chunks_mutex);
            if (m_current_chunks.find(neighbor_key) != m_current_chunks.end())
            {
                m_current_chunks[neighbor_key].need_mesh_update = true;
            }
            //lock.unlock();
        }
    }
}

bool Scene::ShouldRender(int min_xc, int min_zc, int max_xc, int max_zc, Chunk& chunk)
{

    if (chunk.m_x >= min_xc && chunk.m_x <= max_xc && chunk.m_z >= min_zc && chunk.m_z <= max_zc)
        return true;

    return false;
}

Chunk* Scene::GetChunk(std::pair<int, int> chunk_coords)
{
    auto it = m_current_chunks.find(chunk_coords);
    if (it != m_current_chunks.end())
    {
        return &(it->second);
    }
    else
    {
        return nullptr;
    }
}

std::pair<int, int> Scene::GetChunkOffset(int x, int z)
{
    int xc, zc;

    xc = floor(static_cast<float>(x)/CHUNK_SIZE) * CHUNK_SIZE;
    zc = floor(static_cast<float>(z)/CHUNK_SIZE) * CHUNK_SIZE;

    return std::pair<int, int>(xc, zc);
}

Block* Scene::GetBlock(int x, int y, int z)
{
    std::pair<int, int> chunk_coords = GetChunkOffset(x, z);

    Chunk* chunk = GetChunk(chunk_coords);
    if (chunk != nullptr)
    {
        int xc = chunk->m_x;
        int zc = chunk->m_z;

        return chunk->GetBlock(x-xc, y, z-zc);
    }
    else
    {
        return nullptr;
    }
}

Block* Scene::GetRaycastTarget(glm::vec3 ray_origin, glm::vec3 ray_direction)
{

    float t = 0.0f;
    float t_min = 0.0f;
    float t_max = 10.0f;
    float t_step = 0.1f;
    glm::vec3 p;

    while (t < t_max)
    {
        p = ray_origin + ray_direction * t;

        Block* block = GetBlock(p.x, p.y, p.z);
        if (block != nullptr && block->block_type != BlockType::AIR)
        {
            return block;
        }
        t += t_step;
    }

    return nullptr;
}

Scene::~Scene()
{

}
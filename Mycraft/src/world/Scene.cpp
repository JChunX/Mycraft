#include"Scene.h"

Scene::Scene(Camera& camera)
    : m_camera(camera)
{

}

void Scene::Update()
{
    glm::vec3 position = m_camera.position;
    LoadChunks(position.x, position.z);
}

void Scene::LoadChunks(int x, int z)
{
    
    glm::vec2 nearest_chunk_offset = static_cast<float>(CHUNK_SIZE) * glm::vec2(round(x / CHUNK_SIZE), round(z / CHUNK_SIZE));
    int xc0 = nearest_chunk_offset.x;
    int zc0 = nearest_chunk_offset.y;
    //print the xc0, zc0
    std::cout << "xc0: " << xc0 << " zc0: " << zc0 << std::endl;
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
            m_current_chunks.erase(it);
            // print erase
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
                Chunk chunk(xc, zc);
                m_current_chunks.insert({key, chunk});
                //print load 
                std::cout << "load chunk: " << xc << " " << zc << std::endl;
            }
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

Scene::~Scene()
{

}
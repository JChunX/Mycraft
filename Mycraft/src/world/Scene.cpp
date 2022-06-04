#include"Scene.h"

Scene::Scene(Camera& camera)
    : m_camera(camera)
{
    m_current_chunks = std::vector<Chunk>();
    m_occupancy = std::set<std::pair<int, int>>();
}

void Scene::Update()
{
    glm::vec3 position = m_camera.position;
    LoadChunks(position.x, position.z);
}

void Scene::LoadChunks(int x, int z)
{
    
    glm::vec2 nearest_chunk_offset = glm::vec2(floor(x / CHUNK_SIZE), floor(z / CHUNK_SIZE));
    int xc0 = nearest_chunk_offset.x;
    int zc0 = nearest_chunk_offset.y;
    int min_xc = xc0 - CHUNK_LOAD_DISTANCE * CHUNK_SIZE;
    int min_zc = zc0 - CHUNK_LOAD_DISTANCE * CHUNK_SIZE;
    int max_xc = xc0 + (CHUNK_LOAD_DISTANCE-1) * CHUNK_SIZE;
    int max_zc = zc0 + (CHUNK_LOAD_DISTANCE-1) * CHUNK_SIZE;
    
    // unload far chunks
    auto it = m_current_chunks.begin();
    while (it != m_current_chunks.end())
    {
        if (!ShouldRender(min_xc, min_zc, max_xc, max_zc, *it))
        {
            it = m_current_chunks.erase(it);
            m_occupancy.erase(std::pair<int, int>(it->m_x, it->m_z));
        }
        else
        {
            ++it;
        }
    }

    // load chunks
    for (int chunk_idx_x=-CHUNK_LOAD_DISTANCE; chunk_idx_x<CHUNK_LOAD_DISTANCE; chunk_idx_x++)
    {
        for (int chunk_idx_z=-CHUNK_LOAD_DISTANCE; chunk_idx_z<CHUNK_LOAD_DISTANCE; chunk_idx_z++)
        {
            int xc = xc0 + chunk_idx_x * CHUNK_SIZE;
            int zc = zc0 + chunk_idx_z * CHUNK_SIZE;
            // if not in occupancy, load chunk
            if (m_occupancy.find(std::pair<int, int>(xc, zc)) == m_occupancy.end())
            {
                Chunk chunk(xc, zc);
                m_current_chunks.push_back(chunk);
                m_occupancy.insert(std::pair<int, int>(xc, zc));
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


Scene::~Scene()
{

}
#include"Scene.h"
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>

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
            double t0 = glfwGetTime();
            Update();
            double t1 = glfwGetTime();
            std::cout << "+++Update time: " << t1 - t0 << std::endl;
            last_update = glfwGetTime();
        }

    }
}

void Scene::Update()
{
    glm::vec3 position = m_camera.position;
    LoadChunks(position.x, position.z);

    glm::vec3 heading = m_camera.GetHeadingVector();
    auto current_block = GetRaycastTarget(position, heading);

    if (current_block)
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
    
    std::unique_lock<std::mutex> mesh_lock(m_meshes_mutex);
    // remove far chunks
    for (auto it = m_current_chunks.begin(), next_it = it; it != m_current_chunks.end(); it = next_it)
    {
        ++next_it;
        if (!ShouldRender(min_xc, min_zc, max_xc, max_zc, it->second))
        {

            m_meshes.find(it->first)->second.should_erase = true;
            m_fluid_meshes.find(it->first)->second.should_erase = true;
            std::unique_lock<std::mutex> chunk_lock(m_chunks_mutex);
            m_current_chunks.erase(it);
            chunk_lock.unlock();
        }
    }
    mesh_lock.unlock();

    std::vector<std::pair<int,int>> chunk_coords_list;
    for (int chunk_idx_x=-CHUNK_LOAD_DISTANCE; chunk_idx_x<CHUNK_LOAD_DISTANCE; chunk_idx_x++)
    {
        for (int chunk_idx_z=-CHUNK_LOAD_DISTANCE; chunk_idx_z<CHUNK_LOAD_DISTANCE; chunk_idx_z++)
        {

            // only process if xc, zc within chunk load distance radius

            int xc = xc0 + chunk_idx_x * CHUNK_SIZE;
            int zc = zc0 + chunk_idx_z * CHUNK_SIZE;

            if (m_current_chunks.find(std::pair<int,int>(xc,zc)) == m_current_chunks.end())
            {
                chunk_coords_list.push_back(std::make_pair(xc, zc));
            }
        }
    }

    std::cout << "Chunk coords list size: " << chunk_coords_list.size() << std::endl;

    if (chunk_coords_list.size() == 0) {
        return;
    }

    SortChunksByDistance(chunk_coords_list, m_camera.position.x, m_camera.position.z);

    std::vector<std::vector<std::pair<int, int>>> chunk_coords_list_splitted;
    for (int i=0; i<NUM_CHUNK_POOL_WORKERS; i++)
    {
        chunk_coords_list_splitted.push_back(std::vector<std::pair<int, int>>());
    }
    for (int i=0; i<chunk_coords_list.size(); i++)
    {
        chunk_coords_list_splitted[i % NUM_CHUNK_POOL_WORKERS].push_back(chunk_coords_list[i]);
    }

    boost::asio::thread_pool thread_pool(NUM_CHUNK_POOL_WORKERS);
    for (int i=0; i<NUM_CHUNK_POOL_WORKERS; i++)
    {
        boost::asio::post(thread_pool, std::bind(&Scene::LoadChunkAux, this, chunk_coords_list_splitted[i]));
    }
    thread_pool.join();

    for (auto it = m_meshes.begin(); it != m_meshes.end(); it++)
    {
        if (!it->second.should_erase && !it->second.hasmesh)
        {
            mesh_lock.lock();
            it->second.GenerateMesh();
            mesh_lock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    for (auto it = m_fluid_meshes.begin(); it != m_fluid_meshes.end(); it++)
    {
        if (!it->second.should_erase && !it->second.hasmesh)
        {
            mesh_lock.lock();
            it->second.GenerateMesh();
            mesh_lock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

void Scene::SortChunksByDistance(std::vector<std::pair<int, int>>& chunk_coords_list, int x, int z)
{
    std::sort(chunk_coords_list.begin(), chunk_coords_list.end(), [x, z](std::pair<int, int> a, std::pair<int, int> b)
    {
        return glm::distance(glm::vec2(a.first, a.second), glm::vec2(x, z)) < glm::distance(glm::vec2(b.first, b.second), glm::vec2(x, z));
    });
}

void Scene::LoadChunkAux(std::vector<std::pair<int,int>>& chunk_coords_list)
{
    std::cout << "Loading chunks: " << chunk_coords_list.size() << std::endl;
    for (auto it = chunk_coords_list.begin(); it != chunk_coords_list.end(); it++)
    {
        std::pair<int,int> chunk_coords = *it;
        int xc = chunk_coords.first;
        int zc = chunk_coords.second;

        std::unique_lock<std::mutex> chunk_lock(m_chunks_mutex);
        m_current_chunks.try_emplace(chunk_coords, xc, zc);
        
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
                
                if (m_current_chunks.find(neighbor_key) != m_current_chunks.end())
                {
                    m_current_chunks[neighbor_key].need_mesh_update = true;
                }
            }
        }
        chunk_lock.unlock();
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

std::shared_ptr<Block> Scene::GetBlock(int x, int y, int z)
{
    std::pair<int, int> chunk_coords = GetChunkOffset(x, z);

    auto chunk = GetChunk(chunk_coords);
    if (chunk)
    {
        int xc = chunk->m_x;
        int zc = chunk->m_z;

        return chunk->GetBlock(x-xc, y, z-zc);
    }
    else
    {
        return std::shared_ptr<Block>(nullptr);
    }
}

std::shared_ptr<Block> Scene::GetBlock(glm::vec3 pos)
{
    return GetBlock(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(pos.z));
}

std::shared_ptr<Block> Scene::GetRaycastTarget(glm::vec3 ray_origin, glm::vec3 ray_direction)
{

    float t = 0.0f;
    float t_min = 0.0f;
    float t_max = 10.0f;
    float t_step = 0.1f;
    glm::vec3 p;

    while (t < t_max)
    {
        p = ray_origin + ray_direction * t;

        auto block = GetBlock(p.x, p.y, p.z);
        if (block && block->block_type != BlockType::AIR)
        {
            return block;
        }
        t += t_step;
    }

    return std::shared_ptr<Block>(nullptr);
}

Scene::~Scene()
{
}
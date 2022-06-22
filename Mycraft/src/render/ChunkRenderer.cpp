#include"ChunkRenderer.h"
#include"Helpers.h"

ChunkRenderer::ChunkRenderer(Camera& camera, Scene& scene, TextureManager& texture_manager)
    : Renderer(camera, scene, texture_manager, "shaders/block.vert", "shaders/block.frag")
{
    m_shader.SetFloat("air_fog_maxdist", CHUNK_LOAD_DISTANCE * CHUNK_SIZE);
    m_shader.SetFloat("air_fog_mindist", CHUNK_LOAD_DISTANCE * CHUNK_SIZE - 50.0f);
    m_shader.SetVec4("air_fog_color", 0.8f, 0.8f, 0.8f, 0.7f);

    m_shader.SetFloat("water_fog_maxdist", 25.0f);
    m_shader.SetFloat("water_fog_mindist", 1.0f);
    m_shader.SetVec4("water_fog_color", 0.1f, 0.5f, 1.0f, 0.7f);
}

void ChunkRenderer::Render(Scene& scene, TextureManager& texture_manager)
{
    m_shader.Activate();
    if (scene.GetBlock(m_camera.position) && scene.GetBlock(m_camera.position)->block_type == BlockType::WATER)
    {
        m_shader.SetBool("is_underwater", true);
    }
    else
    {
        m_shader.SetBool("is_underwater", false);
    }

    m_camera.SetProjectionViewUniforms(m_shader);


    double t0 = glfwGetTime();
    
    //std::unique_lock<std::mutex> chunk_lock(scene.m_chunks_mutex);
    for (auto& kv : scene.m_current_chunks)
    {
        Chunk* chunk = &(kv.second);

        if (chunk->need_mesh_update) {

            // insert mesh into map
            // try_emplace used here to construct without temporary b/c destruction of temporary creates gl issues with premature vao & vbo deletion
            auto result = scene.m_meshes.try_emplace(kv.first, scene, kv.first, texture_manager);

            if (!result.second) 
            {
                std::unique_lock<std::mutex> mesh_lock(scene.m_meshes_mutex);
                scene.m_meshes.erase(result.first);
                scene.m_meshes.try_emplace(kv.first, scene, kv.first, texture_manager);
            }

            auto result2 = scene.m_fluid_meshes.try_emplace(kv.first, scene, kv.first, texture_manager);
            if (!result2.second) 
            {
                std::unique_lock<std::mutex> mesh_lock(scene.m_meshes_mutex);
                scene.m_fluid_meshes.erase(result2.first);
                scene.m_fluid_meshes.try_emplace(kv.first, scene, kv.first, texture_manager);
            }
        }

        chunk->need_mesh_update = false;
    }
    //chunk_lock.unlock();
    

    for (auto it = scene.m_meshes.begin(), next_it = it; it != scene.m_meshes.end(); it = next_it)
    {
        std::unique_lock<std::mutex> mesh_lock(scene.m_meshes_mutex);
        ++next_it;
        if(it->second.should_erase)
        {
            scene.m_meshes.erase(it);
        }
        else if (!it->second.hasmesh)
        {
            continue;
        }
        else if (!it->second.buffered)
        {
            it->second.Buffer();
        }
        it->second.Render();
    }

    for (auto it = scene.m_fluid_meshes.begin(), next_it = it; it != scene.m_fluid_meshes.end(); it = next_it)
    {
        std::unique_lock<std::mutex> mesh_lock(scene.m_meshes_mutex);
        ++next_it;
        if(it->second.should_erase)
        {
            scene.m_fluid_meshes.erase(it);
            continue;
        }
        else if (!it->second.hasmesh)
        {
            continue;
        }
        else if (!it->second.buffered)
        {
            it->second.Buffer();
        }
        it->second.Render();
    }
}

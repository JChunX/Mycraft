#include"ChunkRenderer.h"
#include"Helpers.h"

ChunkRenderer::ChunkRenderer(Camera& camera, Scene& scene, TextureManager& texture_manager)
    : Renderer(camera, scene, texture_manager, "shaders/block.vert", "shaders/block.frag")
{
    m_shader.SetFloat("air_fog_maxdist", 300.0f);//"air_fog_maxdist", (CHUNK_LOAD_DISTANCE-2) * CHUNK_SIZE);
    m_shader.SetFloat("air_fog_mindistm", 290.0f);//"air_fog_mindist", (CHUNK_LOAD_DISTANCE-5) * CHUNK_SIZE - 50.0f);
    m_shader.SetVec4("air_fog_color", 0.8f, 0.8f, 0.8f, 0.9f);

    m_shader.SetFloat("water_fog_maxdist", 25.0f);
    m_shader.SetFloat("water_fog_mindist", 1.0f);
    m_shader.SetVec4("water_fog_color", 0.1f, 0.5f, 1.0f, 1.0f);
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

    std::unique_lock<std::mutex> chunk_lock(scene.m_chunks_mutex);
    for (auto& kv : scene.m_current_chunks)
    {
        Chunk* chunk = &(kv.second);

        if (chunk->need_mesh_update) {

            // insert mesh into map
            // try_emplace used here to construct without temporary b/c destruction of temporary creates gl issues with premature vao & vbo deletion
            std::unique_lock<std::mutex> mesh_lock(scene.m_meshes_mutex);
            auto result = scene.m_meshes.try_emplace(kv.first, scene, kv.first, texture_manager);

            if (!result.second) 
            {
            //    scene.m_meshes.find(kv.first)->second.GenerateMesh();
                scene.m_meshes.find(kv.first)->second.UpdateBuffer();
            }

            auto result2 = scene.m_fluid_meshes.try_emplace(kv.first, scene, kv.first, texture_manager);

            chunk->need_mesh_update = false;
        }
    }
    chunk_lock.unlock();

    std::unique_lock<std::mutex> mesh_lock(scene.m_meshes_mutex);
    for (auto it = scene.m_meshes.begin(), next_it = it; it != scene.m_meshes.end(); it = next_it)
    {

        ++next_it;
        if(it->second.should_erase)
        {
            scene.m_meshes.erase(it);
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

    for (auto it = scene.m_fluid_meshes.begin(), next_it = it; it != scene.m_fluid_meshes.end(); it = next_it)
    {

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

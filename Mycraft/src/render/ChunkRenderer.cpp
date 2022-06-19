#include"ChunkRenderer.h"
#include"Helpers.h"

ChunkRenderer::ChunkRenderer(Camera& camera)
    : Renderer(camera, "shaders/block.vert", "shaders/block.frag")
{
    m_shader.SetFloat("fog_maxdist", CHUNK_LOAD_DISTANCE * CHUNK_SIZE);
    m_shader.SetFloat("fog_mindist", CHUNK_LOAD_DISTANCE * CHUNK_SIZE - 50.0f);
    m_shader.SetVec4("fog_color", 0.8f, 0.8f, 0.8f, 0.7f);
}

void ChunkRenderer::Render(Scene& scene, TextureManager& texture_manager)
{
    m_shader.Activate();
    m_camera.SetProjectionViewUniforms(m_shader);

    std::unique_lock<std::mutex> chunk_lock(scene.m_chunks_mutex);
    std::unique_lock<std::mutex> mesh_lock(scene.m_meshes_mutex);
    
    for (auto& kv : scene.m_current_chunks)
    {
        Chunk* chunk = &(kv.second);
        if (chunk->need_mesh_update) {

            // insert mesh into map
            // try_emplace used here to construct without temporary b/c destruction of temporary creates gl issues with premature vao & vbo deletion
            auto result = scene.m_meshes.try_emplace(kv.first, scene, kv.first, texture_manager);

            if (!result.second) 
            {
                scene.m_meshes.erase(result.first);
                scene.m_meshes.try_emplace(kv.first, scene, kv.first, texture_manager);;
            }

            chunk->need_mesh_update = false;
        }
    }

    for (auto it = scene.m_meshes.begin(), next_it = it; it != scene.m_meshes.end(); it = next_it)
    {
        ++next_it;
        if(it->second.should_erase)
        {
            scene.m_meshes.erase(it);
        }
        else 
        {
            it->second.Render();
        }
    }
}


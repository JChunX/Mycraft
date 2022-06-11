#include"ChunkRenderer.h"

ChunkRenderer::ChunkRenderer(Camera& camera)
    : m_camera(camera),
      m_shader("shaders/block.vert", "shaders/block.frag")
{

}

ChunkRenderer::~ChunkRenderer()
{

}

void ChunkRenderer::Render(Scene& scene, TextureManager& texture_manager)
{
    m_shader.Activate();
    m_camera.SetProjectionViewUniforms(m_shader);

    std::unique_lock<std::mutex> lock(scene.m_chunks_mutex);
    for (auto& kv : scene.m_current_chunks)
    {
        Chunk* chunk = &(kv.second);
        if (chunk->need_mesh_update) {
            Mesh mesh(scene, kv.first, texture_manager);
            // try inserting mesh into map
            auto result = scene.m_meshes.insert({kv.first, mesh});
            // if key already exists, replace it
            if (!result.second) 
            {
                scene.m_meshes.erase(result.first);
                scene.m_meshes.insert({kv.first, mesh});
            }
            mesh.Render();
            chunk->need_mesh_update = false;
            //print new mesh
            std::cout << "new mesh" << std::endl;
        }
    }
    lock.unlock();

    for (auto& kv : scene.m_meshes)
    {
        kv.second.Render();
    }
}


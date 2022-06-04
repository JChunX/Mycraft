#include"ChunkRenderer.h"

ChunkRenderer::ChunkRenderer(Camera& camera)
    : m_camera(camera),
      m_shader("shaders/block.vert", "shaders/block.frag")
{

}

ChunkRenderer::~ChunkRenderer()
{

}

void ChunkRenderer::Render(std::vector<Chunk>& chunks, Camera& camera)
{
    m_shader.Activate();
    camera.SetProjectionViewUniforms(m_shader);

    for (auto chunk : chunks)
    {
        Mesh mesh(chunk);
        mesh.Render();
    }
}


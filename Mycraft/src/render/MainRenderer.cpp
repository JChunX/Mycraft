#include"MainRenderer.h"

MainRenderer::MainRenderer(Camera& camera) 
    : m_camera(camera),
      m_chunk_renderer(m_camera)
{
   
}

void MainRenderer::Render(Scene& scene)
{
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_chunk_renderer.Render(scene.m_current_chunks, m_camera);
}

MainRenderer::~MainRenderer()
{

}
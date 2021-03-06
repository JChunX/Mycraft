#include"MainRenderer.h"

MainRenderer::MainRenderer(Camera& camera, Scene& scene)
    : m_camera(camera),
      m_scene(scene),
      m_texture_manager(TextureManager()),
      m_chunk_renderer(m_camera, m_scene, m_texture_manager),
      m_skybox_renderer(m_camera, m_scene, m_texture_manager)
{

}

void MainRenderer::Render(Scene& scene)
{
    glClearColor(75.0f/255.0f, 156.0f/255.0f, 211.0f/255.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    m_chunk_renderer.Render(scene, m_texture_manager);
    m_skybox_renderer.Render(scene, m_texture_manager);
}
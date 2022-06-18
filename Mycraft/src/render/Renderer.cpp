#include "Renderer.h"

Renderer::Renderer(Camera& camera, 
                   const char* vertex_path, 
                   const char* fragment_path)
     : m_camera(camera),
       m_shader(vertex_path, fragment_path)
{
    m_shader.Activate();
}

void Renderer::Render(Scene& scene, TextureManager& texture_manager)
{
    m_shader.Activate();
    m_camera.SetProjectionViewUniforms(m_shader);
}
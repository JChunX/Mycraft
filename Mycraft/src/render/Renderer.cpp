#include "Renderer.h"

Renderer::Renderer(Camera& camera, 
                   const char* vertex_path, 
                   const char* fragment_path)
     : m_camera(camera),
       m_shader(Shader(vertex_path, fragment_path))
{
    m_shader.Activate();
}
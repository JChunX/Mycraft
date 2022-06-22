#include "Renderer.h"

Renderer::Renderer(Camera& camera, 
                   Scene& scene,
                   TextureManager& texture_manager,
                   const char* vertex_path, 
                   const char* fragment_path)
     : m_camera(camera),
       m_scene(scene),
       m_texture_manager(texture_manager),
       m_shader(Shader(vertex_path, fragment_path))
{
    m_shader.Activate();
}
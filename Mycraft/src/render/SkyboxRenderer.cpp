#include "SkyboxRenderer.h"

SkyboxRenderer::SkyboxRenderer(Camera& camera, Scene& scene, TextureManager& texture_manager)
    : Renderer(camera, scene, texture_manager, "shaders/skybox.vert", "shaders/skybox.frag"),
      m_skybox(m_texture_manager)
{
    
}

void SkyboxRenderer::Render(Scene& scene, TextureManager& texture_manager)
{
    m_shader.Activate();
    glDepthFunc(GL_LEQUAL);
    // remove translation from the view matrix
    auto view = glm::mat4(glm::mat3(m_camera.GetViewMatrix())); 
    m_shader.SetMat4("view", view);
    m_shader.SetMat4("projection", m_camera.GetProjectionMatrix());
    m_skybox.Render();
    glDepthFunc(GL_LESS);
}
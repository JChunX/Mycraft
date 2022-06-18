#include "SkyboxRenderer.h"

SkyboxRenderer::SkyboxRenderer(Camera& camera)
    : Renderer(camera, "src/shaders/skybox.vert", "src/shaders/skybox.frag")
{
    
}

void SkyboxRenderer::Render(Scene& scene, TextureManager& texture_manager)
{
    
}
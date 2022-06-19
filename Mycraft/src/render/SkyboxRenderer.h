#ifndef SKYBOX_RENDERER_H
#define SKYBOX_RENDERER_H

#include "Renderer.h"
#include "Skybox.h"

class SkyboxRenderer : public Renderer
{
public:
    SkyboxRenderer(Camera& camera, TextureManager& texture_manager);
    void Render(Scene& scene, TextureManager& texture_manager);
private:
    Skybox m_skybox;
};

#endif // SKYBOX_RENDERER_H
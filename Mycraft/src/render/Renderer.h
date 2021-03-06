#ifndef RENDERER_H
#define RENDERER_H

#include "Shader.h"
#include "Camera.h"
#include "TextureManager.h"
#include "Scene.h"

class Renderer
{
public:
    Shader m_shader;
    Camera& m_camera;
    Scene& m_scene;
    TextureManager& m_texture_manager;
    
    Renderer(Camera& camera, Scene& scene, TextureManager& texture_manager, const char* vertex_path, const char* fragment_path);

    virtual ~Renderer() = default;
    virtual void Render(Scene& scene, TextureManager& texture_manager) = 0;
};

#endif // RENDERER_H
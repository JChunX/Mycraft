#ifndef MAINRENDERER_H
#define MAINRENDERER_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"Camera.h"
#include"Scene.h"
#include"ChunkRenderer.h"
#include"SkyboxRenderer.h"

class MainRenderer
{
public:
    Camera& m_camera;
    Scene& m_scene;
    TextureManager m_texture_manager;
    ChunkRenderer m_chunk_renderer;
    SkyboxRenderer m_skybox_renderer;
    
    MainRenderer(Camera& camera, Scene& scene);
    void Render(Scene& scene);
    void Begin(bool* terminate_flag);
};

#endif // MAINRENDERER_H
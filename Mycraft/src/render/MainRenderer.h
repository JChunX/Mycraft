#ifndef MAINRENDERER_H
#define MAINRENDERER_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"Camera.h"
#include"Scene.h"
#include"ChunkRenderer.h"

class MainRenderer
{
public:
    Camera& m_camera;
    ChunkRenderer m_chunk_renderer;
    
    MainRenderer(Camera& camera);
    ~MainRenderer();
    void Render(Scene& scene);
};

#endif
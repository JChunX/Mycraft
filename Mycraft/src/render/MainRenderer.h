#ifndef MAINRENDERER_H
#define MAINRENDERER_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"Camera.h"

class MainRenderer
{
public:
    MainRenderer(Camera& camera);
    ~MainRenderer();
    Camera m_camera;
    void Render();
};

#endif
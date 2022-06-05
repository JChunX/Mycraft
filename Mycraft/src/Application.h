#ifndef APPLICATION_H
#define APPLICATION_H

#include"Camera.h"
#include"Scene.h"
#include"MainRenderer.h"
#include"Context.h"
#include"InputBroadcaster.h"
#include"Helpers.h"

class Application
{
public:
    Context m_context;
    Camera m_camera;
    Scene m_scene;
    MainRenderer m_renderer;
    float m_last_frame;

    Application();
    ~Application();

    void Run();
    float Time();
};

#endif
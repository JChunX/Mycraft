#ifndef APPLICATION_H
#define APPLICATION_H

#include"Camera.h"
//#include"Scene.h"
#include"MainRenderer.h"
#include"Context.h"
#include"InputBroadcaster.h"
#include"Helpers.h"

class Application
{
public:
    Camera m_camera;
    float m_last_frame;
    //Scene m_scene;
    MainRenderer m_renderer;
    Context m_context;

    Application();
    ~Application();

    void Run();
    void FPS();
};

#endif
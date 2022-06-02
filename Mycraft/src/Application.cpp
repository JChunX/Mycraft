#include"Application.h"

Application::Application()
    : m_context(Context()),
      m_camera(Camera(SCR_WIDTH, 
				SCR_HEIGHT, 
				m_context.window, 70.0f, 0.1f, 50.0f, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
      m_renderer(MainRenderer(m_camera))
{

}

void Application::Run()
{
    InputBroadcaster::AddListener(m_camera);
    //m_scene = Scene();
    float m_last_frame = glfwGetTime();
    while (!glfwWindowShouldClose(m_context.window))
    {
        FPS();

        //m_scene.Update();
        m_renderer.Render();

        glfwSwapBuffers(m_context.window);
        glfwPollEvents();
    }
}

void Application::FPS()
{
    float current_frame = glfwGetTime();
    float delta_time = current_frame - m_last_frame;
    m_last_frame = current_frame;
    std::cout << "FPS: " << 1.0f / delta_time << std::endl;
}

Application::~Application()
{

}
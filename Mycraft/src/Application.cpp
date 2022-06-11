#include"Application.h"

Application::Application()
    : m_context(Context()),
      m_camera(Camera(SCR_WIDTH, 
				SCR_HEIGHT, 70.0f, 0.1f, 300.0f, 
                glm::vec3(0.0f, 104.5f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
      m_scene(Scene(m_camera)),
      m_renderer(MainRenderer(m_camera))
{

}

void Application::Run()
{
    InputBroadcaster::AddListener(m_camera);
    glfwSetKeyCallback(m_context.window, ExitCallback);
    
    float m_last_frame = glfwGetTime();

    bool terminate_flag = false;
    std::thread scene_thread(&Scene::Begin, &m_scene, &terminate_flag);

    while (!glfwWindowShouldClose(m_context.window))
    {
        float delta_time = Time();
        log_debug(m_camera);
        InputBroadcaster::ReadInputs(m_context.window, delta_time);
        
        m_renderer.Render(m_scene);
        check_error(1);

        glfwSwapBuffers(m_context.window);
        glfwPollEvents();
    }
    terminate_flag = true;
    scene_thread.join();
}

float Application::Time()
{
    float current_frame = glfwGetTime();
    float delta_time = current_frame - m_last_frame;
    m_last_frame = current_frame;
    //std::cout << "FPS: " << 1.0f / delta_time << std::endl;
    return delta_time;
}

void Application::ExitCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

Application::~Application()
{

}
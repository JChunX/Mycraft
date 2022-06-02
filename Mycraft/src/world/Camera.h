#ifndef CAMERA_H
#define CAMERA_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"WorldObject.h"
#include"Shader.h"
#include"InputListener.h"
#include"InputBroadcaster.h"

class Camera : public WorldObject, public InputListener
{
public:

    float FOVdeg;
    float near_plane;
    float far_plane;

    GLFWwindow *window;

    int width;
    int height;

    bool first_click = true;

    float speed = 0.1f;
    float sensitivity = 100.0f;

    // Camera constructor
    Camera(int width, int height, 
            GLFWwindow* window,
            float FOVdeg=45.0f, 
            float near_plane=0.1f, 
            float far_plane=100.0f,
            glm::vec3 camera_position=glm::vec3(0.0f, 0.0f, 0.0f), 
            glm::vec3 camera_orientation=glm::vec3(0.0f, 0.0f, -1.0f));

    void SetProjectionUniform(Shader& shader, const char* uniform);
    void SetViewUniform(Shader& shader, const char* uniform);

    virtual void Input(UserInput& user_input);
};
#endif
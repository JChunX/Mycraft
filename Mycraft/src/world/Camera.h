#ifndef CAMERA_H
#define CAMERA_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"WorldObject.h"
#include"Shader.h"
#include"InputListener.h"
#include"InputBroadcaster.h"

enum class MotionState {
    SLOW,
    WAIT_FAST_ACTIVATION,
    FAST
};

class Camera : public WorldObject, public InputListener
{
public:

    float FOVdeg;
    float near_plane;
    float far_plane;

    int width;
    int height;

    bool first_click = true;

    float speed = 50.0f;
    float sensitivity = 100.0f;

    float last_time;
    float forward_speed_multiplier = 1.0f;
    bool w_last_pressed = false;
    MotionState motion_state = MotionState::SLOW;

    float dt = 0.0f;

    // Camera constructor
    Camera(int width, int height,
            float FOVdeg=45.0f, 
            float near_plane=0.1f, 
            float far_plane=100.0f,
            glm::vec3 camera_position=glm::vec3(0.0f, 0.0f, 0.0f), 
            glm::vec3 camera_orientation=glm::vec3(0.0f, 0.0f, -1.0f));

    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();
    void SetProjectionUniform(Shader& shader, const char* uniform);
    void SetViewUniform(Shader& shader, const char* uniform);
    void SetProjectionViewUniforms(Shader& shader);

    virtual void Input(GLFWwindow* window, UserInput& user_input, float delta_time);
};
#endif
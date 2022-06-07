#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "WorldObject.h"
#include "Shader.h"
#include "InputListener.h"
#include "InputBroadcaster.h"

#define DEFAULT_FOV 65.0f
#define DEFAULT_NEAR_PLANE 0.1f
#define DEFAULT_FAR_PLANE 100.0f

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

    float speed = 1.0f;
    float sensitivity = 100.0f;

    float last_time;
    float forward_speed_multiplier = 1.0f;
    bool w_last_pressed = false;
    MotionState motion_state = MotionState::SLOW;

    bool is_flying = true;

    // Camera constructor
    Camera(int width, int height,
            float FOVdeg=DEFAULT_FOV, 
            float near_plane=DEFAULT_NEAR_PLANE,
            float far_plane=DEFAULT_FAR_PLANE,
            glm::vec3 camera_position=glm::vec3(0.0f, 100.0f, 0.0f), 
            glm::vec3 camera_orientation=glm::vec3(0.0f, 0.0f, 0.0f));

    void SetProjectionUniform(Shader& shader, const char* uniform);
    void SetViewUniform(Shader& shader, const char* uniform);
    void SetProjectionViewUniforms(Shader& shader);

    virtual void Input(GLFWwindow* window, UserInput& user_input, float delta_time);
    void Tick(float delta_time);
};
#endif
#ifndef INPUTBROADCASTER_H
#define INPUTBROADCASTER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "InputListener.h"
#include "UserInputs.h"

class InputBroadcaster
{
public:
    static float m_last_frame;
    // empty array of InputListeners
    inline static std::vector<InputListener*> listeners;

    // add a listener to the listeners array
    static void AddListener(InputListener& new_listener);
    // remove a listener from the listeners array
    static void RemoveListener(InputListener& to_remove);
    // broadcast the key input to all listeners
    static void ReadInputs(GLFWwindow* window, float delta_time);
};
#endif
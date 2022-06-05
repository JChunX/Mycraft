#include"InputBroadcaster.h"
#include<iostream>

void FPS();

// add a listener to the listeners array
void InputBroadcaster::AddListener(InputListener& new_listener)
{
    listeners.push_back(&new_listener);
}

// remove a listener from the listeners array
void InputBroadcaster::RemoveListener(InputListener& to_remove)
{
    for (int i = 0; i < listeners.size(); i++)
    {
        if (listeners[i]->id == to_remove.id)
        {
            listeners.erase(listeners.begin() + i);
        }
    }
}

// broadcast the key input to all listeners
void InputBroadcaster::ReadInputs(GLFWwindow* window, float delta_time)
{

    // initialize UserInput struct
    UserInput user_input;

    for (int i : actionable_keys)
    {
        KeyInfo key_info;
        if (glfwGetKey(window, i) == GLFW_PRESS)
        {
            key_info = {
                .key = i,
                .key_action = PRESS
            };
            user_input.key_info.push_back(key_info);
        }
        else if (glfwGetKey(window, i) == GLFW_RELEASE)
        {
            key_info = {
                .key = i,
                .key_action = RELEASE
            };
            user_input.key_info.push_back(key_info);
        }
    }

    // get mouse position
    glfwGetCursorPos(window, &(user_input.mouseX), &(user_input.mouseY));

    // get mouse button
    for (int i : actionable_mouse_buttons)
    {
        MouseInfo mouse_info;
        if (glfwGetMouseButton(window, i) == GLFW_PRESS)
        {
            mouse_info = {
                .mouse_action = PRESS,
                .mouse_button = i
            };
            user_input.mouse_info.push_back(mouse_info);
            break;
        }
        else if (glfwGetMouseButton(window, i) == GLFW_RELEASE)
        {
            mouse_info = {
                .mouse_action = RELEASE,
                .mouse_button = i
            };
            user_input.mouse_info.push_back(mouse_info);
        }
    }

    // broadcast user input to all listeners
    for (InputListener* listener : listeners)
    {
        listener->ReceiveInput(window, user_input, delta_time);
    }

}


#ifndef USERINPUTS_H
#define USERINPUTS_H

#include <vector>
#include <GLFW/glfw3.h>

const int actionable_keys[] = {
        GLFW_KEY_W,
        GLFW_KEY_A,
        GLFW_KEY_S,
        GLFW_KEY_D,
        GLFW_KEY_SPACE,
        GLFW_KEY_LEFT_SHIFT,
        GLFW_KEY_LEFT_CONTROL,
    };

const int actionable_mouse_buttons[] = {
        GLFW_MOUSE_BUTTON_LEFT,
        GLFW_MOUSE_BUTTON_RIGHT,
        GLFW_MOUSE_BUTTON_MIDDLE,
};

enum Action {
    PRESS,
    RELEASE
};

struct KeyInfo {
    int key;
    Action key_action;
};

struct MouseInfo{
    Action mouse_action;
    int mouse_button;
};

struct UserInput {
    std::vector<KeyInfo> key_info;
    std::vector<MouseInfo> mouse_info;
    double mouseX;
    double mouseY;
};
#endif
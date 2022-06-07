#ifndef INPUTLISTENER_H
#define INPUTLISTENER_H

#include "UserInputs.h"
#include <iostream>

class InputListener
{
protected:
    // constructor
    InputListener();
public:
    int id;
    virtual ~InputListener() = default;
    void ReceiveInput(GLFWwindow* window, UserInput& user_input, float delta_time);
    virtual void Input(GLFWwindow* window, UserInput& user_input, float delta_time) = 0;
};
#endif
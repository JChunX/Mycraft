#include "InputListener.h"
#include <cstdlib>

InputListener::InputListener()
{
    // generate random id
    this->id = std::rand();
}

void InputListener::ReceiveInput(GLFWwindow* window, UserInput& user_input, float delta_time)
{
    // call the virtual function Input
    this->Input(window, user_input, delta_time);
}
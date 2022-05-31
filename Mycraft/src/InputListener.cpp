#include "InputListener.h"
#include <cstdlib>

InputListener::InputListener()
{
    // generate random id
    this->id = std::rand();
}

void InputListener::ReceiveInput(UserInput& user_input)
{
    // call the virtual function Input
    this->Input(user_input);
}
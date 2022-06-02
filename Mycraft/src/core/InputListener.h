#ifndef INPUTLISTENER_H
#define INPUTLISTENER_H

#include"UserInputs.h"
#include<iostream>

class InputListener
{
protected:
    // constructor
    InputListener();
public:
    int id;
    virtual ~InputListener() = default;
    void ReceiveInput(UserInput& user_input);
    virtual void Input(UserInput& user_input) = 0;
};
#endif
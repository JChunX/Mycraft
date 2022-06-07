#ifndef CONTEXT_H
#define CONTEXT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 720;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

class Context
{
public:
    GLFWwindow* window;
    Context();
    void Destroy();
};

#endif
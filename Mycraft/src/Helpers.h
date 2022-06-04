#ifndef HELPER_H
#define HELPER_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"WorldObject.h"

void check_error(int checkpoint);

void log_debug(WorldObject& object);

#endif
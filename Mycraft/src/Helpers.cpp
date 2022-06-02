#include"Helpers.h"
#include<iostream>

void check_error(int checkpoint)
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "OpenGL Error: " << error << " at checkpoint: " << checkpoint << std::endl;
	}
}
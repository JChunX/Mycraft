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

void log_debug(WorldObject& object)
{
	std::cout << "==== DEBUG ====" << std::endl;
	// print object's position
	std::cout << "Position: " << object.position.x << ", " << object.position.y << ", " << object.position.z << std::endl;
	// print object's rotation, convert rads to degrees
	glm::vec3 rotation = object.GetEulerAngles();
	std::cout << "Rotations: " << rotation.x * 180.0f / glm::pi<float>() << ", " << rotation.y * 180.0f / glm::pi<float>() << ", " << rotation.z * 180.0f / glm::pi<float>() << std::endl;
	// print object's heading vector
	glm::vec3 heading = object.GetHeadingVector();
	std::cout << "Heading: " << heading.x << ", " << heading.y << ", " << heading.z << std::endl;
}
#version 330 core

//Positions/Coordinates
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 normal;
out vec3 fragPos;
out vec3 lightPos;

// Transformation uniforms
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec3 lightPosition;

void main()
{
	// Outputs the positions/coordinates of all vertices
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	normal = mat3(transpose(inverse(view*model))) * aNormal; 
	fragPos = vec3(view * model * vec4(aPos, 1.0));
	lightPos = vec3(view * vec4(lightPosition, 1.0));
}
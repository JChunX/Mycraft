#version 330 core

//Positions/Coordinates
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 normal;
out vec3 fragPos;
out vec3 lightPos;
out vec2 texCoord;

// Transformation uniforms
uniform mat4 projection;
uniform mat4 view;
uniform vec3 lightPosition;

void main()
{
	// Outputs the positions/coordinates of all vertices
	gl_Position = projection * view * vec4(aPos, 1.0);
	normal = mat3(transpose(inverse(view))) * aNormal; 
	fragPos = vec3(view * vec4(aPos, 1.0));
	lightPos = vec3(view * vec4(lightPosition, 1.0));
	texCoord = aTexCoord;
}
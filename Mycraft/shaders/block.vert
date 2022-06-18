#version 330 core

//Positions/Coordinates
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_tex_coord;
layout (location = 3) in vec4 in_recolor;

out vec3 normal;
out vec3 frag_pos;
out vec3 light_pos;
out vec2 tex_coord;
out vec4 recolor;

// Transformation uniforms
uniform mat4 projection;
uniform mat4 view;
uniform vec3 light_position;

void main()
{
	// Outputs the positions/coordinates of all vertices
	gl_Position = projection * view * vec4(in_pos, 1.0);
	normal = mat3(transpose(inverse(view))) * in_normal; 
	frag_pos = vec3(view * vec4(in_pos, 1.0));
	light_pos = vec3(view * vec4(light_position, 1.0));
	tex_coord = in_tex_coord;
	recolor = in_recolor;
}
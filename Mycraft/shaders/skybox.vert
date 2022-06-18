#version 330 core
layout (location = 0) in vec3 in_pos;

out vec3 tex_coords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    tex_coords = in_pos;
    gl_Position = projection * view * vec4(in_pos, 1.0);
}  
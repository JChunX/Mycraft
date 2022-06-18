#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 normal;
in vec3 frag_pos;  
in vec2 tex_coord;
in vec4 recolor;

out vec4 frag_color;

uniform Light light;
uniform Material material;
uniform sampler2D tex0;

uniform float fog_maxdist;
uniform float fog_mindist;
uniform vec4 fog_color;

void main()
{
	float dist = length(frag_pos.xyz);
    float fog_factor = (fog_maxdist - dist) / (fog_maxdist - fog_mindist);
    fog_factor = clamp(fog_factor, 0.0, 1.0);
	vec4 tex_color = texture(tex0, tex_coord) * recolor;
    frag_color = mix(fog_color, tex_color, fog_factor);
}
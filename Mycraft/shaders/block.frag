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

uniform float air_fog_maxdist;
uniform float air_fog_mindist;
uniform vec4 air_fog_color;

uniform float water_fog_maxdist;
uniform float water_fog_mindist;
uniform vec4 water_fog_color;

uniform bool is_underwater;

void main()
{
    vec4 fog_color;
    float fog_maxdist;
    float fog_mindist;

    if (is_underwater) {
        fog_color = water_fog_color;
        fog_maxdist = water_fog_maxdist;
        fog_mindist = water_fog_mindist;
    } else {
        fog_color = air_fog_color;
        fog_maxdist = air_fog_maxdist;
        fog_mindist = air_fog_mindist;
    }

	float dist = length(frag_pos.xyz);
    float fog_factor = (fog_maxdist - dist) / (fog_maxdist - fog_mindist);
    fog_factor = clamp(fog_factor, 0.0, 1.0);
	vec4 tex_color = texture(tex0, tex_coord) * recolor;
    frag_color = mix(fog_color, tex_color, fog_factor);
}
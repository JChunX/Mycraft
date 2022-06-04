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
in vec3 fragPos;  
in vec2 texCoord;

out vec4 fragcolor;

uniform Light light;
uniform Material material;
uniform sampler2D tex0;

void main()
{
	//fragcolor = vec4(result, 1.0);
	fragcolor = texture(tex0, texCoord);
}
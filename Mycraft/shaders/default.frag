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

out vec4 fragcolor;

uniform Light light;
uniform Material material;

void main()
{
	// ambient
	vec3 ambient = material.ambient * light.ambient;

	// diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - fragPos); 
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * light.diffuse * material.diffuse;

	// specular
	vec3 viewDir = normalize(-fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 specular = material.specular * light.specular * pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
	vec3 result = ambient + diffuse + specular;
	fragcolor = vec4(result, 1.0);
}
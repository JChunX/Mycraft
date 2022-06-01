#include"BlockRenderer.h"

BlockRenderer::BlockRenderer(Shader& shader, VBO& vbo, VAO& vao, float* vertices) 
    : shader(shader), 
    vbo(vbo), 
    vao(vao)
{
    
}

void BlockRenderer::Render(Block& block)
{
    shader.Activate();

    glm::vec3 lightPos = glm::vec3(1.2f, 2.0f, 2.0f);
	// Set color uniforms
    shader.SetVec3("light.position", lightPos.x, lightPos.y, lightPos.z);

    // light properties
    glm::vec3 lightColor;
    lightColor.x = static_cast<float>(sin(glfwGetTime() * 2.0));
    lightColor.y = static_cast<float>(sin(glfwGetTime() * 0.7));
    lightColor.z = static_cast<float>(sin(glfwGetTime() * 1.3));
    glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // decrease the influence
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
    shader.SetVec3("light.ambient", ambientColor.x, ambientColor.y, ambientColor.z);
    shader.SetVec3("light.diffuse", diffuseColor.x, diffuseColor.y, diffuseColor.z);
    shader.SetVec3("light.specular", 1.0f, 1.0f, 1.0f);

    // material properties
    shader.SetVec3("material.ambient", 1.0f, 0.5f, 0.31f);
    shader.SetVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
    shader.SetVec3("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
    shader.SetFloat("material.shininess", 32.0f);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, block.position);
    shader.SetMat4("model", model);

    vao.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
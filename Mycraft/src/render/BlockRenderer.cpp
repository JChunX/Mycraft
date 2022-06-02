#include"BlockRenderer.h"

BlockRenderer::BlockRenderer(const char* texture_path, float* vertices, int length)
 : texture_atlas(GL_TEXTURE_2D, texture_path, GL_RGBA, GL_UNSIGNED_BYTE), 
   vbo(vertices, sizeof(float)*length)// sizeof(vertices) here is only the size of pointer, not the size of the array
{
    vao.Bind();
    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3*sizeof(float)));
    vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6*sizeof(float)));
    vao.Unbind();
    vbo.Unbind();
}

void BlockRenderer::Render(Shader& shader, Block& block)
{
    shader.Activate();
    shader.SetVec2("textureOffset", 0.0f, 15.0f / 16);
    texture_atlas.Bind();


    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, block.position);
    shader.SetMat4("model", model);

    vao.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
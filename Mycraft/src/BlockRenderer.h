#ifndef BLOCK_RENDERER_H
#define BLOCK_RENDERER_H

#include"Block.h"
#include"Shader.h"
#include"Texture.h"
#include"VAO.h"
#include"VBO.h"
#include<GLFW/glfw3.h>

class BlockRenderer
{
public:
    Shader shader;

    BlockRenderer(Shader& shader, VBO& vbo, VAO& vao, float* vertices);
    void Render(Block& block);

private:
    VAO vao;
    VBO vbo;
};

#endif
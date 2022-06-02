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

    BlockRenderer(const char* texture_path, float* vertices, int length);
    void Render(Shader& shader, Block& block);

private:
    VAO vao;
    VBO vbo;
    Texture texture_atlas;
};

#endif
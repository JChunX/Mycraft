#ifndef RENDERABLE_H
#define RENDERABLE_H

#include"TextureManager.h"
#include"VAO.h"
#include"VBO.h"

class Renderable
{
public:
    virtual void Render() = 0;
    virtual void Buffer() = 0;
    virtual ~Renderable() = default;
    virtual void Delete();

protected:
    Renderable(TextureManager& texture_manager)
        : m_texture_manager(texture_manager),
          m_vao(VAO()),
          m_vbo(VBO()),
          m_vertices(std::vector<float>()),
          should_erase(false)
    {
    }

    VAO m_vao;
    VBO m_vbo;
    TextureManager& m_texture_manager;
    std::vector<float> m_vertices;
    bool should_erase;
};

void Renderable::Delete()
{
    m_vao.Delete();
    m_vbo.Delete();
}

#endif // RENDERABLE_H
#ifndef MESH_H
#define MESH_H

#include"Chunk.h"
#include"TextureManager.h"
#include"VAO.h"
#include"VBO.h"

class Mesh
{
public:
    VAO m_vao;
    Chunk& m_chunk;
    std::vector<float> m_vertices;
    TextureManager m_texture_manager;
    int n_faces;

    Mesh(Chunk& chunk);
    ~Mesh();
    void Render();
    void GenerateMesh();
    void Buffer();
    void AddFace(Block& block, BlockFace face);
};

#endif // MESH_H
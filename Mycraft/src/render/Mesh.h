#ifndef MESH_H
#define MESH_H

class Scene;

#include"Chunk.h"
#include"TextureManager.h"
#include"VAO.h"
#include"VBO.h"
#include"Scene.h"

class Mesh
{
public:
    VAO m_vao;
    VBO m_vbo;

    Scene& m_scene;
    Chunk& m_chunk;
    TextureManager& m_texture_manager;
    std::vector<float> m_vertices;
    int n_faces;
    bool should_erase;

    Mesh(Scene& scene, std::pair<int, int> chunk_offset, TextureManager& texture_manager);

    ~Mesh();
    void Render();
    void GenerateMesh();
    void Buffer();
    void Delete();
    void AddFace(Block& block, BlockFace face);
};

#endif // MESH_H
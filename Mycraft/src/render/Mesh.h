#ifndef MESH_H
#define MESH_H

class Scene;

#include "Renderable.h"
#include"Chunk.h"
#include"Scene.h"

class Mesh: public Renderable
{
public:

    Scene& m_scene;
    Chunk& m_chunk;
    int n_faces = 0;

    Mesh(Scene& scene, std::pair<int, int> chunk_offset, TextureManager& texture_manager);

    ~Mesh();
    void Render() override;
    void GenerateMesh();
    void Buffer() override;
    void AddFace(Block& block, BlockFace face);
};

#endif // MESH_H
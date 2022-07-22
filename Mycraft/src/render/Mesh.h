#ifndef MESH_H
#define MESH_H

#define NUM_RENDER_POOL_WORKERS 20


class Scene;

#include "Renderable.h"
#include "Chunk.h"
#include <mutex>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>

class Mesh: public Renderable
{
public:
    Scene& m_scene;
    Chunk& m_chunk;
    int n_faces = 0;
    bool hasmesh = false;
    bool buffered = false;

    Mesh(Scene& scene, std::pair<int, int> chunk_offset, TextureManager& texture_manager);

    ~Mesh();
    void Render() override;
    void GenerateMesh();
    void UpdateBuffer();
    void Buffer() override;
    std::vector<BlockFace> DetermineVisibleFaces(Block& block);
    void AddFace(Block& block, BlockFace face);

    std::mutex m_mutex;
protected:
    void GenerateMeshAux(Block& block);
};

#include "Scene.h"

#endif // MESH_H
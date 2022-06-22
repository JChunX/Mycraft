#ifndef CHUNKRENDERER_H
#define CHUNKRENDERER_H


#include "Renderer.h"
#include "Chunk.h"
#include "Mesh.h"
#include "Fluid.h"
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>

class ChunkRenderer : public Renderer
{
public:

    ChunkRenderer(Camera& camera, Scene& scene, TextureManager& texture_manager);
    void Render(Scene& scene, TextureManager& texture_manager);
};

#endif // CHUNKRENDERER_H
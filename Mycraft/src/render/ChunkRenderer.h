#ifndef CHUNKRENDERER_H
#define CHUNKRENDERER_H

#include "Renderer.h"
#include "Chunk.h"
#include "Mesh.h"
#include "Fluid.h"

class ChunkRenderer : public Renderer
{
public:
    ChunkRenderer(Camera& camera);
    void Render(Scene& scene, TextureManager& texture_manager);
};

#endif // CHUNKRENDERER_H
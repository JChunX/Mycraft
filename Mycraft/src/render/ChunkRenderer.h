#ifndef CHUNKRENDERER_H
#define CHUNKRENDERER_H

#include "Shader.h"
#include "Chunk.h"
#include "Mesh.h"
#include "Camera.h"

class ChunkRenderer
{
public:
    Shader m_shader;
    Camera& m_camera;
    ChunkRenderer(Camera& camera);
    ~ChunkRenderer();
    void Render(std::vector<Chunk>& chunks, Camera& camera);
};

#endif // CHUNKRENDERER_H
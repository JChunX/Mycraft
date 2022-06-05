#ifndef CHUNKRENDERER_H
#define CHUNKRENDERER_H

#include "Shader.h"
#include "Chunk.h"
#include "Mesh.h"
#include "Camera.h"
#include "TextureManager.h"

class ChunkRenderer
{
public:
    Shader m_shader;
    Camera& m_camera;
    std::map<std::pair<int, int>, Mesh> m_meshes;
    ChunkRenderer(Camera& camera);
    ~ChunkRenderer();
    void Render(Scene& scene, TextureManager& texture_manager);
};

#endif // CHUNKRENDERER_H
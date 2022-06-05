#ifndef SCENE_H
#define SCENE_H

#define CHUNK_LOAD_DISTANCE 3

#include "Camera.h"
#include "Chunk.h"
#include "Player.h"
#include <map>
#include <thread>

class Scene
{
public:
    Scene(Camera& camera);
    ~Scene();

    std::map<std::pair<int, int>, Chunk> m_current_chunks;
    //Player& m_player;
    Camera& m_camera;

    void Update();
    void LoadChunks(int x, int z);

    bool ShouldRender(int min_xc, 
                    int min_zc, 
                    int max_xc, 
                    int max_zc, 
                    Chunk& chunk);

    Chunk* GetChunk(std::pair<int, int> chunk_coords);
};

#endif // SCENE_H
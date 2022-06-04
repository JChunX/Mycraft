#ifndef SCENE_H
#define SCENE_H

#define CHUNK_LOAD_DISTANCE 1

#include "Camera.h"
#include "Chunk.h"
#include "Player.h"
#include <set>
#include <thread>

class Scene
{
public:
    Scene(Camera& camera);
    ~Scene();

    std::vector<Chunk> m_current_chunks;
    std::set<std::pair<int, int>> m_occupancy;
    Player m_player;
    Camera m_camera;

    void Update();
    void LoadChunks(int x, int z);

    bool ShouldRender(int min_xc, 
                    int min_zc, 
                    int max_xc, 
                    int max_zc, 
                    Chunk& chunk);
};

#endif // SCENE_H
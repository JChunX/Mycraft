#ifndef SCENE_H
#define SCENE_H

#define CHUNK_LOAD_DISTANCE 6

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
    Block* current_block;

    void Update();
    void LoadChunks(int x, int z);

    bool ShouldRender(int min_xc, 
                    int min_zc, 
                    int max_xc, 
                    int max_zc, 
                    Chunk& chunk);

    Chunk* GetChunk(std::pair<int, int> chunk_coords);

    Block* GetBlock(int x, int y, int z);
    Block* GetRaycastTarget(glm::vec3 position, glm::vec3 heading);

};

#endif // SCENE_H
#ifndef SCENE_H
#define SCENE_H

#define NUM_CHUNK_POOL_WORKERS 6

class Mesh; 
class Fluid;

#include "GameParameters.h"
#include "Camera.h"
#include "Chunk.h"
#include "Player.h"
#include "Mesh.h"
#include "Fluid.h"
#include <map>
#include <thread>
#include <mutex>
#include <future>

class Scene
{
public:
    Scene(Camera& camera);
    ~Scene();

    std::map<std::pair<int, int>, Chunk> m_current_chunks;
    std::map<std::pair<int, int>, Mesh> m_meshes;
    std::map<std::pair<int, int>, Fluid> m_fluid_meshes;
    //Player& m_player;
    Camera& m_camera;

    std::mutex m_chunks_mutex;
    std::mutex m_meshes_mutex;

    void Begin(bool* terminate_flag);
    void Update();
    void LoadChunks(int x, int z, glm::vec3 delta);

    bool ShouldRender(int min_xc, 
                    int min_zc, 
                    int max_xc, 
                    int max_zc, 
                    Chunk& chunk);

    Chunk* GetChunk(std::pair<int, int> chunk_coords);
    std::pair<int, int> GetChunkOffset(int x, int z);
    std::shared_ptr<Block> GetBlock(int x, int y, int z);
    std::shared_ptr<Block> GetBlock(glm::vec3 position);
    std::shared_ptr<Block> GetRaycastTarget(glm::vec3 position, glm::vec3 heading);

private:

    glm::vec3 m_camera_position_last = glm::vec3(0.0f, 0.0f, 1.0f);
    void LoadChunkAux(std::vector<std::pair<int,int>>& chunk_coords_list);
    void SortChunksByDistance(std::vector<std::pair<int,int>>& chunk_coords_list, int x, int z);

};

#endif // SCENE_H
#ifndef BLOCK_H
#define BLOCK_H

#include "WorldObject.h"
#include "Shader.h"


enum class BlockType
{
    AIR,
    BEDROCK,
    GRASS,
    DIRT,
    STONE,
    WOOD,
    LEAF,
    SAND,
    WATER,
    NONE,
    GRAVEL
};


enum class BlockFace
{
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
    FRONT,
    BACK
};

class Block : public WorldObject
{
public:

    float temperature;
    float moisture;

    Block(BlockType block_type, glm::vec3 position);
    Block();
    BlockType block_type;
};
#endif
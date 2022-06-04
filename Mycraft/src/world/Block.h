#ifndef BLOCK_H
#define BLOCK_H

#include "WorldObject.h"
#include "Shader.h"


enum class BlockType
{
    AIR,
    GRASS,
    DIRT,
    STONE,
    WOOD,
    LEAF,
    SAND,
    WATER,
    NONE
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
    Block(BlockType block_type, glm::vec3 position);
    Block();
    BlockType block_type;
};
#endif
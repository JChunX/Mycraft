#ifndef BLOCK_H
#define BLOCK_H

#include "WorldObject.h"
#include "Shader.h"
#include "BlockType.h"

class Block : public WorldObject
{
public:
    Block(BlockType block_type, glm::vec3 position);
    BlockType block_type;
};
#endif
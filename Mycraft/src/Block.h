#ifndef BLOCK_H
#define BLOCK_H

#include "WorldObject.h"
#include "Shader.h"

class Block : public WorldObject
{
public:
    Block(glm::vec3 position);
};
#endif
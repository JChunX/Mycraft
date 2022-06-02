#include "Block.h"

Block::Block(BlockType block_type, glm::vec3 position) : WorldObject(position, glm::vec3(0.0f, 0.0f, 0.0f))
{
    this->block_type = block_type;
}
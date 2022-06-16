#include "Block.h"

Block::Block(BlockType block_type, glm::vec3 position)
    : block_type(block_type),
      WorldObject(position, glm::vec3(0.0f, 0.0f, 0.0f))
{
}

Block::Block()
    : block_type(BlockType::NONE),
    WorldObject(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f))
{
}

#include "Block.h"

Block::Block(BlockType block_type, glm::vec3 position)
    : WorldObject(position, glm::vec3(0.0f, 0.0f, 0.0f))
{
    this->block_type = block_type;
}

Block::Block()
    : WorldObject(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f))
{
    this->block_type = BlockType::NONE;
}

void Block::Tick(float delta_time)
{

}
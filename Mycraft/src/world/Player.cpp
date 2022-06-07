#include "Player.h"

Player::Player(Camera& camera)
    : m_camera(camera), 
      WorldObject(camera.position, camera.orientation)
{

}

void Player::Tick(float delta_time)
{
// player should be inputlistener
// also checks for collisions, physics, etc
// then updates the camera using adjusted delta_p
}
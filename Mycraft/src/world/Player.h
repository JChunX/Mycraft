#ifndef PLAYER_H
#define PLAYER_H

#include "Camera.h"

class Player : public WorldObject, public InputListener
{
public:
    Camera& m_camera;
    //Inventory m_inventory;

    Player(Camera& camera);
    void Tick(float delta_time);

};

#endif // PLAYER_H
#ifndef SKYBOX_H
#define SKYBOX_H

#include "TextureManager.h"
#include "Scene.h"

class Skybox: public Renderable
{
public:
    Skybox(TextureManager& texture_manager);
    ~Skybox();
    void Render() override;
    void Buffer() override;
};

#endif // SKYBOX_H
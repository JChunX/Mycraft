#ifndef SKYBOX_TEXTURE_H
#define SKYBOX_TEXTURE_H

#include "Texture.h"

class SkyboxTexture: public Texture
{
public:
    SkyboxTexture();

    void ConfigureTexParameters() override;
};

#endif // SKYBOX_TEXTURE_H


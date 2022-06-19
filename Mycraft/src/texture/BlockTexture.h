#ifndef BLOCK_TEXTURE_H
#define BLOCK_TEXTURE_H

#include "Texture.h"

class BlockTexture: public Texture
{
public:
    BlockTexture(const char* path);

    void ConfigureTexParameters() override;
};

#endif // BLOCK_TEXTURE_H
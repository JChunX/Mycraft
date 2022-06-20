#ifndef FLUID_H
#define FLUID_H

#include "Mesh.h"

class Fluid: public Mesh
{
public:
    Fluid(Scene& scene, std::pair<int, int> chunk_offset, TextureManager& texture_manager);

    void GenerateMesh();
    std::vector<BlockFace> DetermineVisibleFaces(Block& block);
};

#endif // FLUID_H
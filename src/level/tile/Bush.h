#pragma once
#include "level/tile/Tile.h"

class Bush : public Tile
{
public:
    Bush(int id);
    void tick(Level *level, int x, int y, int z, std::default_random_engine &random);
    void render(Tesselator *tess, Level *level, int var3, int x, int y, int z);
    std::optional<AABB> getAABB(int x, int y, int z);
    bool blocksLight();
    bool isSolid();
};

#pragma once
#include "level/tile/Tile.h"

class Bush : public Tile
{
public:
    Bush(int id);
    void tick(Level *level, int x, int y, int z, std::default_random_engine &random) override;
   // void render(Tesselator *tess, Level *level, int var3, int x, int y, int z) override;
    void render(Tesselator &tess, Level *level, int x, int y, int z, int size) override;
    std::optional<AABB> getAABB(int x, int y, int z);
    bool blocksLight();
    bool isSolid();
};

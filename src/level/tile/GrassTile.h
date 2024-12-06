#pragma once
#include "level/tile/Tile.h"
#include <random>

class GrassTile : public Tile
{
public:
    GrassTile(int id);

    void tick(Level *level, int x, int y, int z, std::default_random_engine &random);

protected:
    int getTexture(int face);
};

#ifndef TILE_HPP
#define TILE_HPP

#include "Tesselator.h"
#include "Level.h"

class Tile
{
public:
    // Static instances of Tile
    static Tile rock;
    static Tile grass;

    Tile(int tex);
    void render(Tesselator &tesselator,Level* level, int x, int y, int z, int size);
    void renderFace(Tesselator &tesselator, int x, int y, int z, int face);

private:
    int tex;
};

#endif // TILE_HPP

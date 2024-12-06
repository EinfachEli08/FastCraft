#include "GrassTile.h"

GrassTile::GrassTile(int id) : Tile(id)
{
    tex = 3;
}

int GrassTile::getTexture(int face)
{
    return face == 1 ? 0 : (face == 0 ? 2 : 3);
}

void GrassTile::tick(Level *level, int x, int y, int z, std::default_random_engine &random)
{
    if (!level->isLit(x, y, z))
    {
        level->setTile(x, y, z, Tile::dirt->id);
    }
    else
    {
        for (int i = 0; i < 4; ++i)
        {
            int nx = x + (random() % 3) - 1;
            int ny = y + (random() % 5) - 3;
            int nz = z + (random() % 3) - 1;

            if (level->getTile(nx, ny, nz) == Tile::dirt->id && level->isLit(nx, ny, nz))
            {
                level->setTile(nx, ny, nz, Tile::grass->id);
            }
        }
    }
}

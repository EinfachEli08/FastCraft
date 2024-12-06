#include "Tile.h"
#include <random>

#include "level/tile/DirtTile.h"
#include "level/tile/GrassTile.h"
#include "particle/Particle.h"

// Static tile instances
std::vector<Tile *> Tile::tiles(256, nullptr);
Tile *Tile::empty = nullptr;
Tile *Tile::rock = new Tile(1, 1);
Tile *Tile::grass = new GrassTile(2);
Tile *Tile::dirt = new DirtTile(3, 2);
Tile *Tile::stoneBrick = new Tile(4, 16);
Tile *Tile::wood = new Tile(5, 4);

Tile::Tile(int id) : id(id), tex(0)
{
    tiles[id] = this;
}

Tile::Tile(int id, int tex) : Tile(id)
{
    this->tex = tex;
}

void Tile::render(Tesselator &tess, Level *level, int x, int y, int z, int size)
{
    float brightness = 1.0f;
    float sideBrightness = 0.8f;
    float topBrightness = 0.6f;

    if (this->shouldRenderFace(level, y, z - 1, size, x))
    {
        tess.color(brightness, brightness, brightness);
        this->renderFace(tess, y, z, size, 0);
    }

    if (this->shouldRenderFace(level, y, z + 1, size, x))
    {
        tess.color(brightness, brightness, brightness);
        this->renderFace(tess, y, z, size, 1);
    }

    if (this->shouldRenderFace(level, y, z, size - 1, x))
    {
        tess.color(sideBrightness, sideBrightness, sideBrightness);
        this->renderFace(tess, y, z, size, 2);
    }

    if (this->shouldRenderFace(level, y, z, size + 1, x))
    {
        tess.color(sideBrightness, sideBrightness, sideBrightness);
        this->renderFace(tess, y, z, size, 3);
    }

    if (this->shouldRenderFace(level, y - 1, z, size, x))
    {
        tess.color(topBrightness, topBrightness, topBrightness);
        this->renderFace(tess, y, z, size, 4);
    }

    if (this->shouldRenderFace(level, y + 1, z, size, x))
    {
        tess.color(topBrightness, topBrightness, topBrightness);
        this->renderFace(tess, y, z, size, 5);
    }
}

void Tile::renderFace(Tesselator &tesselator, int x, int y, int z, int face)
{

    int texture = this->getTexture(face);

    float var7 = (float)(texture % 16) / 16.0F;
    float var8 = var7 + 0.999F / 16.0F;
    float var9 = (float)(texture / 16) / 16.0F;
    float var10 = var9 + 0.999F / 16.0F;

    float x1 = x;
    float x2 = x + 1.0f;
    float y1 = y;
    float y2 = y + 1.0f;
    float z1 = z;
    float z2 = z + 1.0f;

    if (face == 0)
    {
        tesselator.vertexUV(x1, y1, z2, var7, var10);
        tesselator.vertexUV(x1, y1, z1, var7, var9);
        tesselator.vertexUV(x2, y1, z1, var8, var9);
        tesselator.vertexUV(x2, y1, z2, var8, var10);
    }

    if (face == 1)
    {
        tesselator.vertexUV(x2, y2, z2, var8, var10);
        tesselator.vertexUV(x2, y2, z1, var8, var9);
        tesselator.vertexUV(x1, y2, z1, var7, var9);
        tesselator.vertexUV(x1, y2, z2, var7, var10);
    }

    if (face == 2)
    {
        tesselator.vertexUV(x1, y2, z1, var8, var9);
        tesselator.vertexUV(x2, y2, z1, var7, var9);
        tesselator.vertexUV(x2, y1, z1, var7, var10);
        tesselator.vertexUV(x1, y1, z1, var8, var10);
    }

    if (face == 3)
    {
        tesselator.vertexUV(x1, y2, z2, var7, var9);
        tesselator.vertexUV(x1, y1, z2, var7, var10);
        tesselator.vertexUV(x2, y1, z2, var8, var10);
        tesselator.vertexUV(x2, y2, z2, var8, var9);
    }

    if (face == 4)
    {
        tesselator.vertexUV(x1, y2, z2, var8, var9);
        tesselator.vertexUV(x1, y2, z1, var7, var9);
        tesselator.vertexUV(x1, y1, z1, var7, var10);
        tesselator.vertexUV(x1, y1, z2, var8, var10);
    }

    if (face == 5)
    {
        tesselator.vertexUV(x2, y1, z2, var7, var10);
        tesselator.vertexUV(x2, y1, z1, var8, var10);
        tesselator.vertexUV(x2, y2, z1, var8, var9);
        tesselator.vertexUV(x2, y2, z2, var7, var9);
    }
}

void Tile::renderFaceNoTexture(Tesselator &tesselator, int x, int y, int z, int face)
{
    float x1 = x;
    float x2 = x + 1.0f;
    float y1 = y;
    float y2 = y + 1.0f;
    float z1 = z;
    float z2 = z + 1.0f;

    if (face == 0)
    {
        tesselator.vertex(x1, y1, z2);
        tesselator.vertex(x1, y1, z1);
        tesselator.vertex(x2, y1, z1);
        tesselator.vertex(x2, y1, z2);
    }

    if (face == 1)
    {
        tesselator.vertex(x2, y2, z2);
        tesselator.vertex(x2, y2, z1);
        tesselator.vertex(x1, y2, z1);
        tesselator.vertex(x1, y2, z2);
    }

    if (face == 2)
    {
        tesselator.vertex(x1, y2, z1);
        tesselator.vertex(x2, y2, z1);
        tesselator.vertex(x2, y1, z1);
        tesselator.vertex(x1, y1, z1);
    }

    if (face == 3)
    {
        tesselator.vertex(x1, y2, z2);
        tesselator.vertex(x1, y1, z2);
        tesselator.vertex(x2, y1, z2);
        tesselator.vertex(x2, y2, z2);
    }

    if (face == 4)
    {
        tesselator.vertex(x1, y2, z2);
        tesselator.vertex(x1, y2, z1);
        tesselator.vertex(x1, y1, z1);
        tesselator.vertex(x1, y1, z2);
    }

    if (face == 5)
    {
        tesselator.vertex(x2, y1, z2);
        tesselator.vertex(x2, y1, z1);
        tesselator.vertex(x2, y2, z1);
        tesselator.vertex(x2, y2, z2);
    }
}

bool Tile::blocksLight()
{
    return true;
}

bool Tile::isSolid()
{
    return true;
}

void Tile::tick(Level *level, int x, int y, int z, std::default_random_engine &random) {}

void Tile::destroy(Level *level, int x, int y, int z, ParticleEngine &particleEngine)
{
    const int particleSize = 4;
    for (int i = 0; i < particleSize; ++i)
    {
        for (int j = 0; j < particleSize; ++j)
        {
            for (int k = 0; k < particleSize; ++k)
            {
                float px = x + (i + 0.5f) / particleSize;
                float py = y + (j + 0.5f) / particleSize;
                float pz = z + (k + 0.5f) / particleSize;
                particleEngine.add(new Particle(level, px, py, pz, px - x - 0.5f, py - y - 0.5f, pz - z - 0.5f, tex));
            }
        }
    }
}

int Tile::getTexture(int face)
{
    return tex;
}

bool Tile::shouldRenderFace(Level *level, int x, int y, int z, int size)
{
    return !level->isSolidTile(x, y, z) && level->isLit(x, y, z) ^ (size == 1);
}

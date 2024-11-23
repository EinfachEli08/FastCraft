#include "Tile.h"

Tile Tile::rock = Tile(0);
Tile Tile::grass = Tile(1);

Tile::Tile(int tex) : tex(tex) {}

void Tile::render(Tesselator &tesselator, Level *level, int size, int x, int y, int z)
{
    float texX = static_cast<float>(this->tex) / 16.0f;
    float texX2 = texX + 0.999f / 16.0f;
    float texY = 0.0f;
    float texY2 = texY + 0.999f / 16.0f;

    float var11 = 1.0F;
    float var12 = 0.8F;
    float var13 = 0.6F;

    // These are the 3D coordinates
    float x1 = x;
    float x2 = x + 1.0f;
    float y1 = y;
    float y2 = y + 1.0f;
    float z1 = z;
    float z2 = z + 1.0f;
    float brightness;
    if (!level->isSolidTile(x, y - 1, z))
    {
        brightness = level->getBrightness(x, y - 1, z) * var11;
        if (brightness == var11 ^ size == 1)
        {
            tesselator.color(brightness, brightness, brightness);
            tesselator.tex(texX, texY2);
            tesselator.vertex(x1, y1, z2);
            tesselator.tex(texX, texY);
            tesselator.vertex(x1, y1, z1);
            tesselator.tex(texX2, texY);
            tesselator.vertex(x2, y1, z1);
            tesselator.tex(texX2, texY2);
            tesselator.vertex(x2, y1, z2);
        }
    }

    if (!level->isSolidTile(x, y + 1, z))
    {
        brightness = level->getBrightness(x, y, z) * var11;
        if (brightness == var11 ^ size == 1)
        {
            tesselator.color(brightness, brightness, brightness);
            tesselator.tex(texX2, texY2);
            tesselator.vertex(x2, y2, z2);
            tesselator.tex(texX2, texY);
            tesselator.vertex(x2, y2, z1);
            tesselator.tex(texX, texY);
            tesselator.vertex(x1, y2, z1);
            tesselator.tex(texX, texY2);
            tesselator.vertex(x1, y2, z2);
        }
    }

    if (!level->isSolidTile(x, y, z - 1))
    {
        brightness = level->getBrightness(x, y, z - 1) * var12;
        if (brightness == var11 ^ size == 1)
        {
            tesselator.color(brightness, brightness, brightness);
            tesselator.tex(texX2, texY);
            tesselator.vertex(x1, y2, z1);
            tesselator.tex(texX, texY);
            tesselator.vertex(x2, y2, z1);
            tesselator.tex(texX, texY2);
            tesselator.vertex(x2, y1, z1);
            tesselator.tex(texX2, texY2);
            tesselator.vertex(x1, y1, z1);
        }
    }

    if (!level->isSolidTile(x, y, z + 1))
    {
        brightness = level->getBrightness(x, y, z + 1) * var12;
        if (brightness == var11 ^ size == 1)
        {
            tesselator.color(brightness, brightness, brightness);
            tesselator.tex(texX, texY);
            tesselator.vertex(x1, y2, z2);
            tesselator.tex(texX, texY2);
            tesselator.vertex(x1, y1, z2);
            tesselator.tex(texX2, texY2);
            tesselator.vertex(x2, y1, z2);
            tesselator.tex(texX2, texY);
            tesselator.vertex(x2, y2, z2);
        }
    }

    if (!level->isSolidTile(x - 1, y, z))
    {
        brightness = level->getBrightness(x - 1, y, z) * var13;
        if (brightness == var11 ^ size == 1)
        {
            tesselator.color(brightness, brightness, brightness);
            tesselator.tex(texX2, texY);
            tesselator.vertex(x1, y2, z2);
            tesselator.tex(texX, texY);
            tesselator.vertex(x1, y2, z1);
            tesselator.tex(texX, texY2);
            tesselator.vertex(x1, y1, z1);
            tesselator.tex(texX2, texY2);
            tesselator.vertex(x1, y1, z2);
        }
    }

    if (!level->isSolidTile(x + 1, y, z))
    {
        brightness = level->getBrightness(x + 1, y, z) * var13;
        if (brightness == var11 ^ size == 1)
        {
            tesselator.color(100, 100, 100);
            tesselator.tex(texX, texY2);
            tesselator.vertex(x2, y1, z2);
            tesselator.tex(texX2, texY2);
            tesselator.vertex(x2, y1, z1);
            tesselator.tex(texX2, texY);
            tesselator.vertex(x2, y2, z1);
            tesselator.tex(texX, texY);
            tesselator.vertex(x2, y2, z2);
        }
    }
}
void Tile::renderFace(Tesselator &tesselator, int x, int y, int z, int face)
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

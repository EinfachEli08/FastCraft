#include "Tile.h"

Tile Tile::rock = Tile(0);
Tile Tile::grass = Tile(1);

Tile::Tile(int tex) : tex(tex) {}

void Tile::render(Tesselator &tesselator, int size, int x, int y, int z)
{
    float texX = static_cast<float>(this->tex) / 16.0f;
    float texX2 = texX + 0.999f / 16.0f;
    float texY = 0.0f;
    float texY2 = texY + 0.999f / 16.0f;

    // These are the 3D coordinates
    float x1 = x;
    float x2 = x + 1.0f;
    float y1 = y;
    float y2 = y + 1.0f;
    float z1 = z;
    float z2 = z + 1.0f;

    tesselator.color(1, 1, 1);
    tesselator.tex(texX, texY2);
    tesselator.vertex(x1, y1, z2);
    tesselator.tex(texX, texY);
    tesselator.vertex(x1, y1, z1);
    tesselator.tex(texX2, texY);
    tesselator.vertex(x2, y1, z1);
    tesselator.tex(texX2, texY2);
    tesselator.vertex(x2, y1, z2);

    tesselator.color(1, 1, 1);
    tesselator.tex(texX2, texY2);
    tesselator.vertex(x2, y2, z2);
    tesselator.tex(texX2, texY);
    tesselator.vertex(x2, y2, z1);
    tesselator.tex(texX, texY);
    tesselator.vertex(x1, y2, z1);
    tesselator.tex(texX, texY2);
    tesselator.vertex(x1, y2, z2);

    tesselator.color(1, 1, 1);
    tesselator.tex(texX2, texY);
    tesselator.vertex(x1, y2, z1);
    tesselator.tex(texX, texY);
    tesselator.vertex(x2, y2, z1);
    tesselator.tex(texX, texY2);
    tesselator.vertex(x2, y1, z1);
    tesselator.tex(texX2, texY2);
    tesselator.vertex(x1, y1, z1);

    tesselator.color(100, 100, 100);
    tesselator.tex(texX, texY);
    tesselator.vertex(x1, y2, z2);
    tesselator.tex(texX, texY2);
    tesselator.vertex(x1, y1, z2);
    tesselator.tex(texX2, texY2);
    tesselator.vertex(x2, y1, z2);
    tesselator.tex(texX2, texY);
    tesselator.vertex(x2, y2, z2);

    tesselator.color(100, 100, 100);
    tesselator.tex(texX2, texY);
    tesselator.vertex(x1, y2, z2);
    tesselator.tex(texX, texY);
    tesselator.vertex(x1, y2, z1);
    tesselator.tex(texX, texY2);
    tesselator.vertex(x1, y1, z1);
    tesselator.tex(texX2, texY2);
    tesselator.vertex(x1, y1, z2);

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

void Tile::renderFace(Tesselator &tesselator, int x, int y, int z)
{
    float x1 = x;
    float x2 = x + 1.0f;
    float y1 = y;
    float y2 = y + 1.0f;
    float z1 = z;
    float z2 = z + 1.0f;

    // Simple face rendering, assuming one side
    tesselator.vertex(x1, y1, z2);
    tesselator.vertex(x1, y1, z1);
    tesselator.vertex(x2, y1, z1);
    tesselator.vertex(x2, y1, z2);

    tesselator.vertex(x2, y2, z2);
    tesselator.vertex(x2, y2, z1);
    tesselator.vertex(x1, y2, z1);
    tesselator.vertex(x1, y2, z2);

    tesselator.vertex(x1, y2, z1);
    tesselator.vertex(x2, y2, z1);
    tesselator.vertex(x2, y1, z1);
    tesselator.vertex(x1, y1, z1);

    tesselator.vertex(x1, y2, z2);
    tesselator.vertex(x1, y1, z2);
    tesselator.vertex(x2, y1, z2);
    tesselator.vertex(x2, y2, z2);

    tesselator.vertex(x1, y2, z2);
    tesselator.vertex(x1, y2, z1);
    tesselator.vertex(x1, y1, z1);
    tesselator.vertex(x1, y1, z2);

    tesselator.vertex(x2, y1, z2);
    tesselator.vertex(x2, y1, z1);
    tesselator.vertex(x2, y2, z1);
    tesselator.vertex(x2, y2, z2);
}

#include "Tile.h"

Tile Tile::rock = Tile(0);
Tile Tile::grass = Tile(1);

Tile::Tile(int tex) : tex(tex) {}

void Tile::render(Tesselator &tesselator, int x, int y, int z, int size)
{
    float texX = static_cast<float>(this->tex) / 16.0f;
    float texX2 = texX + 0.999f / 16.0f;
    float texY = 0.0f;
    float texY2 = texY + 0.999f / 16.0f;

    // These are the 3D coordinates
    float x1 = static_cast<float>(x);
    float x2 = x1 + 1.0f;
    float y1 = static_cast<float>(y);
    float y2 = y1 + 1.0f;
    float z1 = static_cast<float>(z);
    float z2 = z1 + 1.0f;

    // Front face
    tesselator.color(1.0f, 1.0f, 1.0f);
    tesselator.tex(texX, texY2);
    tesselator.vertex(x1, y1, z2);
    tesselator.tex(texX, texY);
    tesselator.vertex(x1, y1, z1);
    tesselator.tex(texX2, texY);
    tesselator.vertex(x2, y1, z1);
    tesselator.tex(texX2, texY2);
    tesselator.vertex(x2, y1, z2);

    // Back face
    tesselator.tex(texX2, texY2);
    tesselator.vertex(x2, y2, z2);
    tesselator.tex(texX2, texY);
    tesselator.vertex(x2, y2, z1);
    tesselator.tex(texX, texY);
    tesselator.vertex(x1, y2, z1);
    tesselator.tex(texX, texY2);
    tesselator.vertex(x1, y2, z2);

    // Left face
    tesselator.tex(texX2, texY);
    tesselator.vertex(x1, y2, z1);
    tesselator.tex(texX, texY);
    tesselator.vertex(x2, y2, z1);
    tesselator.tex(texX, texY2);
    tesselator.vertex(x2, y1, z1);
    tesselator.tex(texX2, texY2);
    tesselator.vertex(x1, y1, z1);

    // Right face
    tesselator.tex(texX, texY2);
    tesselator.vertex(x1, y2, z2);
    tesselator.tex(texX, texY);
    tesselator.vertex(x1, y1, z2);
    tesselator.tex(texX2, texY);
    tesselator.vertex(x2, y1, z2);
    tesselator.tex(texX2, texY2);
    tesselator.vertex(x2, y2, z2);

    // Bottom face
    tesselator.tex(texX, texY);
    tesselator.vertex(x1, y1, z1);
    tesselator.tex(texX, texY2);
    tesselator.vertex(x1, y2, z1);
    tesselator.tex(texX2, texY2);
    tesselator.vertex(x2, y2, z1);
    tesselator.tex(texX2, texY);
    tesselator.vertex(x2, y1, z1);

    // Top face
    tesselator.tex(texX2, texY2);
    tesselator.vertex(x1, y2, z2);
    tesselator.tex(texX2, texY);
    tesselator.vertex(x1, y1, z2);
    tesselator.tex(texX, texY);
    tesselator.vertex(x2, y1, z2);
    tesselator.tex(texX, texY2);
    tesselator.vertex(x2, y2, z2);
}

void Tile::renderFace(Tesselator &tesselator, int x, int y, int z)
{
    float x1 = static_cast<float>(x);
    float x2 = x1 + 1.0f;
    float y1 = static_cast<float>(y);
    float y2 = y1 + 1.0f;
    float z1 = static_cast<float>(z);
    float z2 = z1 + 1.0f;

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

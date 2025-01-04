#pragma once

#include "Polygon.h"
#include <vector>

class Cube
{
private:
    std::vector<Vertex> vertices;
    std::vector<Polygon> polygons;
    int textureOffsetX;
    int textureOffsetY;
    int list = 0;
    bool compiled = false;
    void compile();

public:
    float x, y, z;
    float xRot, yRot, zRot;

    Cube(int textureOffsetX, int textureOffsetY);

    void setTexOffs(int textureOffsetX, int textureOffsetY);

    void addBox(float x, float y, float z, int width, int height, int depth);

    void setPos(float x, float y, float z);

    void render();
};
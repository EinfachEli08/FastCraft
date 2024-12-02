#pragma once

#include "Polygon.h"
#include <vector>

class Cube
{
private:
    std::vector<Vertex> vertices;
    std::vector<Polygon> polygons;
    int xTexOffs;
    int yTexOffs;

public:
    float x, y, z;         
    float xRot, yRot, zRot; 

    Cube(int xTexOffs, int yTexOffs);

    void setTexOffs(int var1, int var2);

    void addBox(float x, float y, float z, int width, int height, int depth);

    void setPos(float x, float y, float z);

    void render() const;
};

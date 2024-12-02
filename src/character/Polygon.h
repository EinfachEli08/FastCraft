#pragma once
#include "Vertex.h"
#include <vector>

class Polygon
{
public:
    std::vector<Vertex> vertices;
    int xTexOffs1, yTexOffs1, xTexOffs2, yTexOffs2;

    Polygon() : xTexOffs1(0), yTexOffs1(0), xTexOffs2(0), yTexOffs2(0) {}

    Polygon(const std::vector<Vertex> &var1);
    Polygon(const std::vector<Vertex> &var1, int var2, int var3, int var4, int var5);
    void render() const;
};

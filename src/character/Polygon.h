#pragma once
#include "Vertex.h"
#include <vector>

class Polygon
{
public:
    std::vector<Vertex> vertices; 
    int vertexCount;

    Polygon() = default; 
    Polygon(const std::vector<Vertex> &vertices);
    Polygon(const std::vector<Vertex> &vertices, int x1, int y1, int x2, int y2);

    void render() const;
};

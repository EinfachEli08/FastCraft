#pragma once
#include "Vec3.h"

class Vertex
{
public:
    Vec3 pos;
    float u;
    float v;
    Vertex() : pos(0.0f, 0.0f, 0.0f), u(0.0f), v(0.0f) {}

    Vertex(float x, float y, float z, float u, float v);
    Vertex(const Vertex &other, float u, float v);
    Vertex(const Vec3 &position, float u, float v);

    Vertex remap(float u, float v) const;
};
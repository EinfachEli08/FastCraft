#include "Vertex.h"

Vertex::Vertex(float x, float y, float z, float u, float v)
    : pos(x, y, z), u(u), v(v) {}

Vertex::Vertex(const Vertex &other, float u, float v)
    : pos(other.pos), u(u), v(v) {}

Vertex::Vertex(const Vec3 &position, float u, float v)
    : pos(position), u(u), v(v) {}

Vertex Vertex::remap(float u, float v) const
{
    return Vertex(*this, u, v);
}
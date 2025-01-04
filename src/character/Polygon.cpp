#include "Polygon.h"
#include <glad/glad.h>

Polygon::Polygon(const std::vector<Vertex> &vertices) : vertices(vertices) {}

Polygon::Polygon(const std::vector<Vertex> &vertices, int x1, int y1, int x2, int y2) : vertices(vertices)
{

    this->vertices[0] = this->vertices[0].remap(static_cast<float>(x1), static_cast<float>(y1));
    this->vertices[1] = this->vertices[1].remap(static_cast<float>(x2), static_cast<float>(y1));
    this->vertices[2] = this->vertices[2].remap(static_cast<float>(x2), static_cast<float>(y2));
    this->vertices[3] = this->vertices[3].remap(static_cast<float>(x1), static_cast<float>(y2));
}


void Polygon::render() const
{
    glBegin(GL_QUADS);
    for (const auto &vertex : vertices)
    {
        glTexCoord2f(vertex.u / 64.0f, vertex.v / 32.0f);
        glVertex3f(vertex.pos.x, vertex.pos.y, vertex.pos.z);
    }
    glEnd();
}
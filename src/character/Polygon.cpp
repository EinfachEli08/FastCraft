#include "Polygon.h"
#include <glad/glad.h>

Polygon::Polygon(const std::vector<Vertex> &vertices)
    : vertices(vertices), vertexCount(vertices.size()) {}

Polygon::Polygon(const std::vector<Vertex> &vertices, int x1, int y1, int x2, int y2)
    : vertices(vertices), vertexCount(vertices.size())
{
    std::vector<Vertex> modifiableVertices = vertices; // Create a copy

    if (modifiableVertices.size() >= 4)
    {
        modifiableVertices[0] = modifiableVertices[0].remap(static_cast<float>(x1), static_cast<float>(y1));
        modifiableVertices[1] = modifiableVertices[1].remap(static_cast<float>(x2), static_cast<float>(y1));
        modifiableVertices[2] = modifiableVertices[2].remap(static_cast<float>(x2), static_cast<float>(y2));
        modifiableVertices[3] = modifiableVertices[3].remap(static_cast<float>(x1), static_cast<float>(y2));
    }

    // Now you can use modifiableVertices
    this->vertices = modifiableVertices;
}

void Polygon::render() const
{
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
    for (int i = 3; i >= 0; --i)
    {
        const Vertex &var2 = vertices[i];
        glTexCoord2f(var2.u / 63.999F, var2.v / 31.999F);
        glVertex3f(var2.pos.x, var2.pos.y, var2.pos.z);
    }
    glEnd(); // End drawing
}

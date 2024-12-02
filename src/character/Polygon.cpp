#include "Polygon.h"
#include <glad/glad.h>

Polygon::Polygon(const std::vector<Vertex> &var1) : vertices(var1) {}

Polygon::Polygon(const std::vector<Vertex> &var1, int var2, int var3, int var4, int var5)
    : vertices(var1)
{
    if (vertices.size() >= 4)
    {
        vertices[0].u = static_cast<float>(var4);
        vertices[0].v = static_cast<float>(var3);

        vertices[1].u = static_cast<float>(var2);
        vertices[1].v = static_cast<float>(var3);

        vertices[2].u = static_cast<float>(var2);
        vertices[2].v = static_cast<float>(var5);

        vertices[3].u = static_cast<float>(var4);
        vertices[3].v = static_cast<float>(var5);
    }
}

void Polygon::render() const
{
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
    for (int i = 3; i >= 0; --i)
    {
        const Vertex &var2 = vertices[i];
        glTexCoord2f(var2.u / 64.0f, var2.v / 32.0f);
        glVertex3f(var2.x, var2.y, var2.z);
    }
    glEnd(); // End drawing
}

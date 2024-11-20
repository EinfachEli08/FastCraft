#include "Tesselator.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <GLFW/glfw3.h>

Tesselator::Tesselator() : vertices(0), u(0.0f), v(0.0f), r(1.0f), g(1.0f), b(1.0f), hasColor(false), hasTexture(false)
{
    vertexBuffer.reserve(MAX_VERTICES * 3);
    texCoordBuffer.reserve(MAX_VERTICES * 2);
    colorBuffer.reserve(MAX_VERTICES * 3);
}

Tesselator::~Tesselator()
{
    clear();
}

void Tesselator::init()
{
    clear();
    hasColor = false;
    hasTexture = false;
}

void Tesselator::tex(float u, float v)
{
    hasTexture = true;
    this->u = u;
    this->v = v;
}

void Tesselator::color(float r, float g, float b)
{
    hasColor = true;
    this->r = r;
    this->g = g;
    this->b = b;
}

void Tesselator::vertex(float x, float y, float z)
{
    vertexBuffer.push_back(x);
    vertexBuffer.push_back(y);
    vertexBuffer.push_back(z);

    if (hasTexture)
    {
        texCoordBuffer.push_back(u);
        texCoordBuffer.push_back(v);
    }

    if (hasColor)
    {
        colorBuffer.push_back(r);
        colorBuffer.push_back(g);
        colorBuffer.push_back(b);
    }

    ++vertices;

    if (vertices >= MAX_VERTICES)
    {
        flush();
    }
}

void Tesselator::flush()
{
    if (vertices == 0)
        return;

    glEnableClientState(GL_VERTEX_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, vertexBuffer.data());

    if (hasTexture)
    {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, texCoordBuffer.data());
    }

    if (hasColor)
    {
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(3, GL_FLOAT, 0, colorBuffer.data());
    }

    glDrawArrays(GL_QUADS, 0, vertices);

    glDisableClientState(GL_VERTEX_ARRAY);

    if (hasTexture)
    {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    if (hasColor)
    {
        glDisableClientState(GL_COLOR_ARRAY);
    }

    clear();
}

void Tesselator::clear()
{
    vertices = 0;
    vertexBuffer.clear();
    texCoordBuffer.clear();
    colorBuffer.clear();
}

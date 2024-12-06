#include "Tesselator.h"
#include <GLFW/glfw3.h>
#include <iostream>

Tesselator Tesselator::instance;

Tesselator::Tesselator()
    : buffer(MAX_FLOATS, 0.0f), vertices(0), u(0.0f), v(0.0f),
      r(1.0f), g(1.0f), b(1.0f), hasColor(false), hasTexture(false),
      len(3), p(0) {}

Tesselator::~Tesselator()
{
    this->clear();
}

Tesselator &Tesselator::getInstance()
{
    return instance;
}

void Tesselator::init()
{
    this->clear();
    this->hasColor = false;
    this->hasTexture = false;
}

void Tesselator::tex(float u, float v)
{
    if (!this->hasTexture)
    {
        this->len += 2;
    }
    this->hasTexture = true;
    this->u = u;
    this->v = v;
}

void Tesselator::color(float r, float g, float b)
{
    if (!this->hasColor)
    {
        this->len += 3;
    }
    this->hasColor = true;
    this->r = r;
    this->g = g;
    this->b = b;
}

void Tesselator::vertex(float x, float y, float z)
{
    if (this->hasTexture)
    {
        this->buffer[this->p++] = this->u;
        this->buffer[this->p++] = this->v;
    }

    if (this->hasColor)
    {
        this->buffer[this->p++] = this->r;
        this->buffer[this->p++] = this->g;
        this->buffer[this->p++] = this->b;
    }

    this->buffer[this->p++] = x;
    this->buffer[this->p++] = y;
    this->buffer[this->p++] = z;

    ++this->vertices;

    if (this->p >= MAX_FLOATS - this->len)
    {
        this->flush();
    }
}

void Tesselator::vertexUV(float x, float y, float z, float u, float v)
{
    this->tex(u, v);
    this->vertex(x, y, z);
}

void Tesselator::flush()
{
    if (this->vertices == 0)
        return;

    glEnableClientState(GL_VERTEX_ARRAY);

    if (this->hasTexture && this->hasColor)
    {
        glInterleavedArrays(GL_T2F_C3F_V3F, 0, this->buffer.data());
    }
    else if (this->hasTexture)
    {
        glInterleavedArrays(GL_T2F_V3F, 0, this->buffer.data());
    }
    else if (this->hasColor)
    {
        glInterleavedArrays(GL_C3F_V3F, 0, this->buffer.data());
    }
    else
    {
        glInterleavedArrays(GL_V3F, 0, this->buffer.data());
    }

    glDrawArrays(GL_QUADS, 0, this->vertices);

    glDisableClientState(GL_VERTEX_ARRAY);

    if (this->hasTexture)
    {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    if (this->hasColor)
    {
        glDisableClientState(GL_COLOR_ARRAY);
    }

    this->clear();
}

void Tesselator::clear()
{
    this->vertices = 0;
    this->p = 0;
    this->buffer.clear();
    this->buffer.resize(MAX_FLOATS, 0.0f); // Reset the buffer
}

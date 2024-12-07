#include "Tesselator.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstring>

Tesselator Tesselator::instance;

Tesselator::Tesselator()
    : buffer(MAX_FLOATS, 0.0f), array(MAX_FLOATS,0.0f), vertices(0), u(0.0f), v(0.0f),
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

void Tesselator::flush()
{
    this->buffer.clear();
    if (buffer.size() < p)
    {
        buffer.resize(p);
    }
    std::memcpy(buffer.data(), array.data(), p * sizeof(float));
    buffer.clear();

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

    glEnableClientState(GL_VERTEX_ARRAY);
    if (this->hasTexture)
    {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    if (this->hasColor)
    {
        glEnableClientState(GL_COLOR_ARRAY);
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
    this->buffer.clear();
    this->p = 0;
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

void Tesselator::vertexUV(float x, float y, float z, float u, float v)
{
    this->tex(u, v);
    this->vertex(x, y, z);
}

void Tesselator::vertex(float x, float y, float z)
{
    if (this->hasTexture)
    {
        this->array[this->p++] = this->u;
        this->array[this->p++] = this->v;
    }

    if (this->hasColor)
    {
        this->array[this->p++] = this->r;
        this->array[this->p++] = this->g;
        this->array[this->p++] = this->b;
    }

    this->array[this->p++] = x;
    this->array[this->p++] = y;
    this->array[this->p++] = z;

    ++this->vertices;

    if (this->p >= MAX_FLOATS - this->len)
    {
        this->flush();
    }
}

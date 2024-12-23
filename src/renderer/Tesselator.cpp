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
    if (this->vertices == 0)
        return; // Keine Daten, keine Operation.

    // Direkt mit array.data() arbeiten
    if (this->hasTexture && this->hasColor)
    {
        glInterleavedArrays(GL_T2F_C3F_V3F, 0, this->array.data());
    }
    else if (this->hasTexture)
    {
        glInterleavedArrays(GL_T2F_V3F, 0, this->array.data());
    }
    else if (this->hasColor)
    {
        glInterleavedArrays(GL_C3F_V3F, 0, this->array.data());
    }
    else
    {
        glInterleavedArrays(GL_V3F, 0, this->array.data());
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
    this->hasNoColor = false;
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
    if(!this->hasNoColor){
        if (!this->hasColor)
        {
            this->len += 3;
        }
        this->hasColor = true;
        this->r = r;
        this->g = g;
        this->b = b;
    }

}

void Tesselator::vertexUV(float x, float y, float z, float u, float v)
{
    this->tex(u, v);
    this->vertex(x, y, z);
}

void Tesselator::vertex(float x, float y, float z)
{
    float *ptr = this->array.data() + this->p;
    if (this->hasTexture)
    {
        *ptr++ = this->u;
        *ptr++ = this->v;
    }
    if (this->hasColor)
    {
        *ptr++ = this->r;
        *ptr++ = this->g;
        *ptr++ = this->b;
    }
    *ptr++ = x;
    *ptr++ = y;
    *ptr++ = z;

    this->p += (ptr - (this->array.data() + this->p)); // Update Index
    ++this->vertices;

    if (this->vertices % 4 == 0 && this->p >= MAX_FLOATS - this->len*4)
    {
        this->flush();
    }
}

void Tesselator::color(int hex)
{
    float var2 = (float)(hex >> 16 & 255) / 255.0F;
    float var3 = (float)(hex >> 8 & 255) / 255.0F;
    float var4 = (float)(hex & 255) / 255.0F;
    this->color(var2, var3, var4);
}

void Tesselator::noColor()
{
   this->hasNoColor = true;
}
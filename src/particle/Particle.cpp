#include "Particle.h"
#include <cmath>
#include <cstdlib>

Particle::Particle(Level *level, float x, float y, float z, float xd, float yd, float zd, int tex)
    : Entity(level), tex(tex)
{
    setSize(0.2f, 0.2f);
    heightOffset = bbHeight / 2.0f;
    setPos(x, y, z);
    this->xd = xd + (rand() / float(RAND_MAX) * 2.0f - 1.0f) * 0.4f;
    this->yd = yd + (rand() / float(RAND_MAX) * 2.0f - 1.0f) * 0.4f;
    this->zd = zd + (rand() / float(RAND_MAX) * 2.0f - 1.0f) * 0.4f;

    float var9 = (rand() / float(RAND_MAX) + rand() / float(RAND_MAX) + 1.0f) * 0.15f;
    float var10 = std::sqrt(xd * xd + yd * yd + zd * zd);
    this->xd = xd / var10 * var9 * 0.7f;
    this->yd = yd / var10 * var9;
    this->zd = zd / var10 * var9 * 0.7f;

    uo = rand() / float(RAND_MAX) * 3.0f;
    vo = rand() / float(RAND_MAX) * 3.0f;
}

void Particle::tick()
{
    xo = x;
    yo = y;
    zo = z;
    if (rand() / float(RAND_MAX) < 0.1f)
    {
        remove();
    }

    yd -= 0.06f;
    move(xd, yd, zd);
    xd *= 0.98f;
    yd *= 0.98f;
    zd *= 0.98f;

    if (onGround)
    {
        xd *= 0.7f;
        zd *= 0.7f;
    }
}

void Particle::render(Tesselator &tesselator, float deltaTime, float var2, float var3, float var4)
{
    float var6 = ((tex % 16) + uo / 4.0f) / 16.0f;
    float var7 = var6 + 0.999f / 64.0f;
    float var8 = ((tex / 16) + vo / 4.0f) / 16.0f;
    float var9 = var8 + 0.999f / 64.0f;
    float var10 = 0.1f;
    float var11 = xo + (x - xo) * deltaTime;
    float var12 = yo + (y - yo) * deltaTime;
    float var13 = zo + (z - zo) * deltaTime;

    tesselator.vertexUV(var11 - var2 * var10, var12 - var3 * var10, var13 - var4 * var10, var6, var9);
    tesselator.vertexUV(var11 - var2 * var10, var12 + var3 * var10, var13 - var4 * var10, var6, var8);
    tesselator.vertexUV(var11 + var2 * var10, var12 + var3 * var10, var13 + var4 * var10, var7, var8);
    tesselator.vertexUV(var11 + var2 * var10, var12 - var3 * var10, var13 + var4 * var10, var7, var9);
}

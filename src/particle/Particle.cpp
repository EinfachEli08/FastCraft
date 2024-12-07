#include "Particle.h"
#include <cmath>
#include <cstdlib>

Particle::Particle(Level *level, float x, float y, float z, float xd, float yd, float zd, int tex)
    : Entity(level), tex(tex)
{
    this->setSize(0.2f, 0.2f);
    this->heightOffset = this->bbHeight / 2.0f;
    this->setPos(x, y, z);
    this->xd = xd + (Math::random() * 2.0f - 1.0f) * 0.4f;
    this->yd = yd + (Math::random() * 2.0f - 1.0f) * 0.4f;
    this->zd = zd + (Math::random() * 2.0f - 1.0f) * 0.4f;

    float var9 = (Math::random() + Math::random() + 1.0f) * 0.15f;
    float var10 = std::sqrt(this->xd * this->xd + this->yd * this->yd + this->zd * this->zd);
    this->xd = this->xd / var10 * var9 * 0.7f;
    this->yd = this->yd / var10 * var9;
    this->zd = this->zd / var10 * var9 * 0.7f;

    uo = Math::random() * 3.0f;
    vo = Math::random() * 3.0f;
}

void Particle::tick()
{
    this->xo = this->x;
    this->yo = this->y;
    this->zo = this->z;
    if (Math::random() < 0.1D)
    {
        this->remove();
    }

    this->yd = this->yd - 0.06D;
    this->move(this->xd, this->yd, this->zd);
    this->xd *= 0.98f;
    this->yd *= 0.98f;
    this->zd *= 0.98f;

    if (this->onGround)
    {
        this->xd *= 0.7f;
        this->zd *= 0.7f;
    }
}

void Particle::render(Tesselator &tesselator, float deltaTime, float var2, float var3, float var4)
{
    float var6 = ((this->tex % 16) + this->uo / 4.0f) / 16.0f;
    float var7 = var6 + 0.999f / 64.0f;
    float var8 = ((this->tex / 16) + this->vo / 4.0f) / 16.0f;
    float var9 = var8 + 0.999f / 64.0f;
    float var10 = 0.1f;
    float var11 = this->xo + (this->x - this->xo) * deltaTime;
    float var12 = this->yo + (this->y - this->yo) * deltaTime;
    float var13 = this->zo + (this->z - this->zo) * deltaTime;

    tesselator.vertexUV(var11 - var2 * var10, var12 - var3 * var10, var13 - var4 * var10, var6, var9);
    tesselator.vertexUV(var11 - var2 * var10, var12 + var3 * var10, var13 - var4 * var10, var6, var8);
    tesselator.vertexUV(var11 + var2 * var10, var12 + var3 * var10, var13 + var4 * var10, var7, var8);
    tesselator.vertexUV(var11 + var2 * var10, var12 - var3 * var10, var13 + var4 * var10, var7, var9);
}

#include "Particle.h"
#include <cmath>
#include <cstdlib>

Particle::Particle(Level *level, float x, float y, float z, float xd, float yd, float zd, int tex)
    : Entity(level), tex(tex)
{
    this->setSize(0.2f, 0.2f);
    this->heightOffset = this->bbHeight / 2.0f;
    
    this->setPos(x, y, z);

    this->xd = xd + (Math::random() * 2.0D - 1.0D) * 0.4f;
    this->yd = yd + (Math::random() * 2.0D - 1.0D) * 0.4f;
    this->zd = zd + (Math::random() * 2.0D - 1.0D) * 0.4f;

    float var9 = (float)(Math::random() + Math::random() + 1.0f) * 0.15f;

    float var10 = (float)std::sqrt((double)(this->xd * this->xd + this->yd * this->yd + this->zd * this->zd));

    this->xd = this->xd / var10 * var9 * 0.4f;
    this->yd = this->yd / var10 * var9 * 0.4f + 0.1F;
    this->zd = this->zd / var10 * var9 * 0.4f;

    uo = Math::random() * 3.0f;
    vo = Math::random() * 3.0f;

    this->size = (float)(Math::random() * 0.5D + 0.5D);
    this->lifetime = (int)(4.0D / (Math::random() * 0.9D + 0.1D));
    this->age = 0;
}

void Particle::tick()
{
    this->xo = this->x;
    this->yo = this->y;
    this->zo = this->z;
    if (this->age++ >= this->lifetime)
    {
        this->remove();
    }

    this->yd = (float)((double)this->yd - 0.04D);
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

void Particle::render(Tesselator &var1, float var2, float var3, float var4, float var5, float var6, float var7)
{
    float var8 = ((float)(this->tex % 16) + this->uo / 4.0F) / 16.0F;
    float var9 = var8 + 0.999F / 64.0F;
    float var10 = ((float)(this->tex / 16) + this->vo / 4.0F) / 16.0F;
    float var11 = var10 + 0.999F / 64.0F;
    float var12 = 0.1F * this->size;
    float var13 = this->xo + (this->x - this->xo) * var2;
    float var14 = this->yo + (this->y - this->yo) * var2;
    float var15 = this->zo + (this->z - this->zo) * var2;
    var1.vertexUV(var13 - var3 * var12 - var6 * var12, var14 - var4 * var12, var15 - var5 * var12 - var7 * var12, var8, var11);
    var1.vertexUV(var13 - var3 * var12 + var6 * var12, var14 + var4 * var12, var15 - var5 * var12 + var7 * var12, var8, var10);
    var1.vertexUV(var13 + var3 * var12 + var6 * var12, var14 + var4 * var12, var15 + var5 * var12 + var7 * var12, var9, var10);
    var1.vertexUV(var13 + var3 * var12 - var6 * var12, var14 - var4 * var12, var15 + var5 * var12 - var7 * var12, var9, var11);
}
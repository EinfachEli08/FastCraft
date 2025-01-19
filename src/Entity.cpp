#include "Entity.h"
#include "utils/Math.h"
#include <algorithm>

Entity::Entity(Level *level) : level(level), onGround(false), heightOffset(0.0f)
{
    this->xo = this->yo = this->zo = 0.0f;
    this->x = this->y = this->z = 0.0f;
    this->xd = this->yd = this->zd = 0.0f;
    this->yRot = 0.0f;
    this->xRot = 0.0f;
    this->resetPos();
}

void Entity::resetPos()
{
    float var1 = (float)Math::random() * (float)this->level->width;
    float var2 = (float)(this->level->depth + 10);
    float var3 = (float)Math::random() * (float)this->level->height;
    this->setPos(var1, var2, var3);
}

void Entity::remove()
{
    this->removed = true;
}

void Entity::setSize(float bbWidth, float bbHeight)
{
    this->bbWidth = bbWidth;
    this->bbHeight = bbHeight;
}

void Entity::setPos(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
    float var4 = this->bbWidth / 2.0F;
    float var5 = this->bbHeight / 2.0F;
    this->bb = AABB(x - var4, y - var5, z - var4, x + var4, y + var5, z + var4);
}

void Entity::turn(float yRotDelta, float xRotDelta)
{
    this->yRot = (float)((double)this->yRot + (double)yRotDelta * 0.15D);
    this->xRot = (float)((double)this->xRot - (double)xRotDelta * 0.15D);

    if (this->xRot < -90.0f)
    {
        this->xRot = -90.0f;
    }

    if (this->xRot > 90.0f)
    {
        this->xRot = 90.0f;
    }
}

void Entity::tick()
{
    this->xo = this->x;
    this->yo = this->y;
    this->zo = this->z;
}

void Entity::move(float dx, float dy, float dz)
{
    float var4 = dx;
    float var5 = dy;
    float var6 = dz;

    std::vector<AABB> cubes = this->level->getCubes(this->bb.expand(dx, dy, dz));

    for (auto &cube : cubes)
    {
        dy = cube.clipYCollide(this->bb, dy);
    }

    this->bb.move(0.0f, dy, 0.0f);

    for (auto &cube : cubes)
    {
        dx = cube.clipXCollide(this->bb, dx);
    }

    this->bb.move(dx, 0.0f, 0.0f);

    for (auto &cube : cubes)
    {
        dz = cube.clipZCollide(this->bb, dz);
    }

    this->bb.move(0.0f, 0.0f, dz);
    this->onGround = (var5 != dy && var5 < 0.0f);
    if (var4 != dx)
    {
        this->xd = 0.0f;
    }

    if (var5 != dy)
    {
        this->yd = 0.0f;
    }

    if (var6 != dz)
    {
        this->zd = 0.0f;
    }

    this->x = (this->bb.x0 + this->bb.x1) / 2.0f;
    this->y = this->bb.y0 + this->heightOffset;
    this->z = (this->bb.z0 + this->bb.z1) / 2.0f;
}

void Entity::moveRelative(float x, float z, float speed)
{
    float len = x * x + z * z;
    if (len >= 0.01f)
    {
        len = speed / (float)std::sqrt((double)len);
        x *= len;
        z *= len;

        float sinYaw = (float)std::sin((double)this->yRot * Math::PI / 180.0f);
        float cosYaw = (float)std::cos((double)this->yRot * Math::PI / 180.0f);

        this->xd += x * cosYaw - z * sinYaw;
        this->zd += z * cosYaw + x * sinYaw;
    }
}

bool Entity::isLit()
{
    int x = this->x;
    int y = this->y;
    int z = this->z;
    return this->level->isLit(x, y, z);
}

void Entity::render(float partialTicks)
{
}
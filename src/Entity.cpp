#include "Entity.h"
#include <cmath>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Constructor
Entity::Entity(Level *level) : level(level), onGround(false), heightOffset(0.0f)
{
    this->resetPos();
}

// Reset position of the entity
void Entity::resetPos()
{
    float var1 = static_cast<float>(std::rand()) / RAND_MAX * this->level->width;
    float var2 = static_cast<float>(this->level->depth + 10);
    float var3 = static_cast<float>(std::rand()) / RAND_MAX * this->level->height;
    this->setPos(var1, var2, var3);
}

// Set position and update bounding box
void Entity::setPos(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
    float var4 = 0.3f;
    float var5 = 0.9f;
    this->bb = AABB(x - var4, y - var5, z - var4, x + var4, y + var5, z + var4);
}

// Rotate the entity
void Entity::turn(float yRot, float xRot)
{
    this->yRot += yRot * 0.15f;
    this->xRot -= xRot * 0.15f;

    if (this->xRot < -90.0f)
        this->xRot = -90.0f;
    if (this->xRot > 90.0f)
        this->xRot = 90.0f;
}

// Update tick state
void Entity::tick()
{
    this->xo = this->x;
    this->yo = this->y;
    this->zo = this->z;
}

// Move entity based on collisions and position change
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
        this->xd = 0.0f;
    if (var5 != dy)
        this->yd = 0.0f;
    if (var6 != dz)
        this->zd = 0.0f;

    this->x = (this->bb.x0 + this->bb.x1) / 2.0f;
    this->y = this->bb.y0 + this->heightOffset;
    this->z = (this->bb.z0 + this->bb.z1) / 2.0f;
}

// Move the entity relative to its rotation
void Entity::moveRelative(float x, float z, float speed)
{
    float len = x * x + z * z;
    if (len >= 0.01f)
    {
        len = speed / std::sqrt(len);
        x *= len;
        z *= len;

        float sinYaw = std::sin(yRot * M_PI / 180.0f);
        float cosYaw = std::cos(yRot * M_PI / 180.0f);

        this->xd += x * cosYaw - z * sinYaw;
        this->zd += z * cosYaw + x * sinYaw;
    }
}

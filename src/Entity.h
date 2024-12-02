#pragma once

#include "level/Level.h"
#include <vector>
#include <cmath>
#include <cstdlib>

class Entity
{
private:
public:
    void resetPos();
    Level *level;
    float xo, yo, zo;
    float x, y, z;
    float xd, yd, zd;
    float yRot, xRot;
    AABB bb;
    bool onGround;
    float heightOffset;

    Entity(Level *level);

    void turn(float yRot, float xRot);
    void setPos(float x, float y, float z);
    void tick();
    virtual void move(float dx, float dy, float dz);
    virtual void moveRelative(float x, float z, float speed);
};

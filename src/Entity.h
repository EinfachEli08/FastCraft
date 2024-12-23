#pragma once

#include "level/Level.h"
#include "utils/Math.h"
#include <vector>
#include <cmath>
#include <cstdlib>

class Entity
{
protected:
    Level *level;
    float heightOffset = 0.0;
    void setPos(float x, float y, float z);
    void setSize(float bbWidth, float bbHeight);

private:
public:
    Entity(Level *level);

    void resetPos();

    float xo, yo, zo;
    float x, y, z;
    float xd, yd, zd;
    float yRot, xRot;
    AABB bb;
    bool onGround = false;
    bool removed = false;
    float bbWidth = 0.6;
    float bbHeight = 1.8;

    void turn(float yRot, float xRot);

    void tick();
    virtual void move(float dx, float dy, float dz);
    virtual void moveRelative(float x, float z, float speed);
    void render(float partialTicks);
    void remove();
    bool isLit();
};

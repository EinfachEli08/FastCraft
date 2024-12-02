#pragma once

#include "Entity.h"
#include "Cube.h"
#include "utils/Math.h"
#include <cmath>
#include <cstdlib>

class Zombie : public Entity
{
public:
    Cube head;
    Cube body;
    Cube arm0;
    Cube arm1;
    Cube leg0;
    Cube leg1;
    float rot;
    float timeOffs;
    float speed;
    float rotA;

    Zombie(Level *level, float x, float y, float z);

    void tick();
    void render(float partialTick);
};

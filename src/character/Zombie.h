#pragma once

#include "Entity.h"
#include "character/Cube.h"
#include "utils/Math.h"
#include "character/ZombieModel.h"
#include <cmath>
#include <cstdlib>

class Zombie : public Entity
{
private:
    static ZombieModel zombieModel;

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

#pragma once

#include "character/Cube.h"
#include "utils/Math.h"
#include <chrono>
#include <cmath>


class ZombieModel
{
public:
    Cube head;
    Cube body;
    Cube arm0;
    Cube arm1;
    Cube leg0;
    Cube leg1;

    ZombieModel();

    void render(float speed, float timeOffs);
};

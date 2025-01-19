#pragma once

#include "phys/AABB.h"
#include "LevelListener.h"
#include "level/NoiseMap.h"

#include <random>
#include <vector>
#include <fstream>
#include <iostream>

class LevelGen
{
public:
    LevelGen(int width, int height, int depth);

    int width;
    int height;
    int depth;
    std::mt19937 random;
};


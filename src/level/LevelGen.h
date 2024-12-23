#pragma once

#include "phys/AABB.h"
#include "LevelListener.h"
#include "level/PerlinNoiseFilter.h"

#include <random>
#include <vector>
#include <fstream>
#include <iostream>

class LevelGen
{
private:
    int width;
    int height;
    int depth;
    std::mt19937 random;

public:
    LevelGen(int width, int height, int depth);
    std::vector<unsigned int> generateMap();
};


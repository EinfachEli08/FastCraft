// LevelGen.cpp
#include "level/LevelGen.h"
#include "level/tile/Tile.h"
#include "utils/Math.h"
#include <cmath>
#include <algorithm>

LevelGen::LevelGen(int width, int height, int depth)
    : width(width), height(height), depth(depth), random(std::random_device{}()) {}


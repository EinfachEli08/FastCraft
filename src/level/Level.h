#pragma once

#include "phys/AABB.h"
#include "LevelListener.h"
#include "level/PerlinNoiseFilter.h"

#include <random>
#include <vector>
#include <fstream>
#include <iostream>

class Level
{
public:
    int width;
    int height;
    int depth;

    Level(int width, int height, int depth);
    bool load();
    void save();
    void calcLightDepths(int var1, int var2, int var3, int var4);
    void addListener(LevelListener *listener);
    void removeListener(LevelListener *listener);
    bool isTile(int x, int y, int z);
    bool isSolidTile(int x, int y, int z);
    bool isLightBlocker(int x, int y, int z);
    std::vector<AABB> getCubes(const AABB &aabb);
    float getBrightness(int x, int y, int z);
    bool setTile(int x, int y, int z, int tileId);
    bool isLit(int x, int y, int z);
    int getTile(int x, int y, int z);
    void tick();

private:
    std::vector<unsigned int> blocks;
    std::vector<int> lightDepths;
    std::vector<LevelListener *> levelListeners;

    int unprocessed;
    std::random_device rd; // Declare random_device
    std::mt19937 random;   // Declare mt19937
};

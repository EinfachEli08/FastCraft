#pragma once
#include "phys/AABB.h"
#include "level/NoiseMap.h"

#include <random>
#include <vector>
#include <fstream>
#include <iostream>

// Forward declaration of Fastcraft
class Fastcraft;
class LevelRenderer;

class Level
{
public:
    int width;
    int height;
    int depth;

    Level(Fastcraft* fc, int width, int height, int depth);
    void generateMap();
    bool load();
    void save();
    void calcLightDepths(int var1, int var2, int var3, int var4);
    bool isSolidTile(int x, int y, int z);
    bool setTileNoUpdate(int x, int y, int z, int tileId);
    bool isLightBlocker(int x, int y, int z);
    std::vector<AABB> getCubes(const AABB &aabb);

    bool setTile(int x, int y, int z, int tileId);
    bool isLit(int x, int y, int z);
    int getTile(int x, int y, int z);
    void tick();
    bool containsLiquid(AABB* aabb, int type);

    std::vector<LevelRenderer *> levelListeners;

private:
    Fastcraft* fastCraft;
    std::vector<unsigned int> blocks;
    std::vector<int> lightDepths;


    std::vector<int> coords;

    int unprocessed;
    std::random_device rd; // Declare random_device
    std::mt19937 random;   // Declare mt19937

    void updateNeighbourAt(int x, int y, int z, int tileId);
    long floodFill(int var1, int var2, int var3, int var4, int var5);
    void calculateLightDepths(int x, int y, int width, int height);
};
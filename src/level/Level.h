#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "../phys/AABB.h"
#include "LevelListener.h"
#include <vector>
#include <fstream>
#include <iostream>


class Level
{
public:
    int width;
    int height;
    int depth;
    std::vector<unsigned int> blocks;
    std::vector<int> lightDepths;
    std::vector<LevelListener *> levelListeners;

    Level();
    void addListener(LevelListener *listener);
    void removeListener(LevelListener *listener);
    bool isTile(int x, int y, int z) const;
    bool isSolidTile(int x, int y, int z) const;
    bool isLightBlocker(int x, int y, int z) const;
    std::vector<AABB> getCubes(const AABB &aabb) const;
    float getBrightness(int x, int y, int z) const;
    void setTile(int x, int y, int z, int tileId);
};

#endif // LEVEL_HPP

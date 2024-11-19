#include "Level.h"
#include "LevelListener.h"

#include <algorithm>

Level::Level()
    : width(16), height(16), depth(16)
{

    // Initialize blocks and lightDepths
    blocks.resize(width * height * depth);
    lightDepths.resize(width * depth);

    for (int x = 0; x < width; ++x)
    {
        for (int z = 0; z < depth; ++z)
        {
            for (int y = 0; y < height; ++y)
            {
                int index = (z * height + y) * width + x;
                blocks[index] = (z <= depth * 2 / 3) ? 1 : 0; // Set rock/grass
            }
        }
    }
}

void Level::addListener(LevelListener *listener)
{
    levelListeners.push_back(listener);
}

void Level::removeListener(LevelListener *listener)
{
    levelListeners.erase(std::remove(levelListeners.begin(), levelListeners.end(), listener), levelListeners.end());
}

bool Level::isTile(int x, int y, int z) const
{
    return (x >= 0 && y >= 0 && z >= 0 && x < width && y < height && z < depth) &&
           blocks[(y * depth + z) * width + x] == 1;
}

bool Level::isSolidTile(int x, int y, int z) const
{
    return isTile(x, y, z);
}

bool Level::isLightBlocker(int x, int y, int z) const
{
    return isSolidTile(x, y, z);
}

std::vector<AABB> Level::getCubes(const AABB &aabb) const
{
    std::vector<AABB> cubes;
    int minX = static_cast<int>(aabb.x0);
    int maxX = static_cast<int>(aabb.x1 + 1.0f);
    int minY = static_cast<int>(aabb.y0);
    int maxY = static_cast<int>(aabb.y1 + 1.0f);
    int minZ = static_cast<int>(aabb.z0);
    int maxZ = static_cast<int>(aabb.z1 + 1.0f);

    minX = std::max(minX, 0);
    minY = std::max(minY, 0);
    minZ = std::max(minZ, 0);
    maxX = std::min(maxX, width);
    maxY = std::min(maxY, height);
    maxZ = std::min(maxZ, depth);

    for (int x = minX; x < maxX; ++x)
    {
        for (int y = minY; y < maxY; ++y)
        {
            for (int z = minZ; z < maxZ; ++z)
            {
                if (isSolidTile(x, y, z))
                {
                    cubes.emplace_back(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z),
                                       static_cast<float>(x + 1), static_cast<float>(y + 1), static_cast<float>(z + 1));
                }
            }
        }
    }

    return cubes;
}

float Level::getBrightness(int x, int y, int z) const
{
    const float darkBrightness = 0.8f;
    const float brightBrightness = 1.0f;

    if (x >= 0 && y >= 0 && z >= 0 && x < width && y < height && z < depth)
    {
        return (y < lightDepths[x + z * width]) ? darkBrightness : brightBrightness;
    }
    return brightBrightness;
}

void Level::setTile(int x, int y, int z, int tileId)
{
    if (x >= 0 && y >= 0 && z >= 0 && x < width && y < height && z < depth)
    {
        blocks[(y * depth + z) * width + x] = static_cast<unsigned char>(tileId);

        // Notify listeners
        for (auto *listener : levelListeners)
        {
            listener->tileChanged(x, y, z);
        }
    }
}

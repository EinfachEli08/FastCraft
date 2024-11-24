#include "Level.h"

#include <algorithm>
#include <cmath>
#include <zlib.h>
#include <functional>
#include <fstream>

Level::Level(int width, int height, int depth) : width(0), height(0), depth(0), blocks(0), lightDepths()
{

    this->width = width;
    this->height = height;
    this->depth = depth;

    this->blocks.resize(width * height * depth);
    this->lightDepths.resize(width * depth);

    for (int x = 0; x < width; ++x)
    {
        for (int z = 0; z < depth; ++z)
        {
            for (int y = 0; y < height; ++y)
            {
                int index = (z * this->height + y) * this->width + x;
                this->blocks[index] = (z <= depth * 2 / 3) ? 1 : 0; // Set rock/grass
            }
        }
    }
    calcLightDepths(0, 0, width, height);
    load();
}

void Level::load()
{
    try
    {
        // Read the file
        std::ifstream file("level.dat", std::ios::binary);
        if (!file)
        {
            throw std::runtime_error("Unable to open file for reading.");
        }

        std::vector<char> compressedData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        // Decompress
        uLongf decompressedSize = this->blocks.size() * sizeof(int);
        if (uncompress(reinterpret_cast<Bytef *>(this->blocks.data()), &decompressedSize,
                       reinterpret_cast<const Bytef *>(compressedData.data()), compressedData.size()) != Z_OK)
        {
            throw std::runtime_error("Failed to decompress block data.");
        }

        if (decompressedSize != this->blocks.size() * sizeof(int))
        {
            throw std::runtime_error("Decompressed data size mismatch.");
        }

        // Recalculate lighting for the entire level
        calcLightDepths(0, 0, this->width, this->depth);

        for (auto &listener : this->levelListeners)
        {
            listener->allChanged();
        }

        std::cout << "Level loaded successfully." << std::endl;
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error loading level: " << ex.what() << std::endl;
    }
}

void Level::save()
{
    try
    {
        // Compress the entire block data
        uLongf compressedSize = compressBound(this->blocks.size() * sizeof(int));
        std::vector<char> compressedData(compressedSize);

        if (compress(reinterpret_cast<Bytef *>(compressedData.data()), &compressedSize,
                     reinterpret_cast<const Bytef *>(this->blocks.data()), this->blocks.size() * sizeof(int)) != Z_OK)
        {
            throw std::runtime_error("Failed to compress block data.");
        }
        compressedData.resize(compressedSize); // Adjust size to actual compressed data

        // Save to file
        std::ofstream file("level.dat", std::ios::binary);
        if (!file)
        {
            throw std::runtime_error("Unable to open file for writing.");
        }

        file.write(compressedData.data(), compressedData.size());
        file.close();
        std::cout << "Level saved successfully." << std::endl;
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error saving level: " << ex.what() << std::endl;
    }
}

void Level::calcLightDepths(int x, int y, int width, int height)
{
    for (int var5 = x; var5 < x + width; ++var5)
    {
        for (int var6 = y; var6 < y + height; ++var6)
        {

            int var7 = this->lightDepths[var5 + var6 * this->width];
            int var8;
            for (var8 = this->depth - 1; var8 > 0 && !this->isLightBlocker(var5, var8, var6); --var8)
            {
            }

            this->lightDepths[var5 + var6 * this->width] = var8;

            if (var7 != var8)
            {
                int var9 = var7 < var8 ? var7 : var8;
                int var10 = var7 > var8 ? var7 : var8;

                for (int var11 = 0; var11 < this->levelListeners.size(); ++var11)
                {
                    this->levelListeners.at(var11)->lightColumnChanged(var5, var6, var9, var10);
                }
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
    return (x >= 0 && y >= 0 && z >= 0 && x < this->width && y < this->depth && z < this->height) && this->blocks[(y * this->height + z) * this->width + x] == 1;
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
    int minX = aabb.x0;
    int maxX = aabb.x1 + 1.0f;
    int minY = aabb.y0;
    int maxY = aabb.y1 + 1.0f;
    int minZ = aabb.z0;
    int maxZ = aabb.z1 + 1.0f;

    minX = std::max(minX, 0);
    minY = std::max(minY, 0);
    minZ = std::max(minZ, 0);
    maxX = std::min(maxX, this->width);
    maxY = std::min(maxY, this->depth);
    maxZ = std::min(maxZ, this->height);

    for (int x = minX; x < maxX; ++x)
    {
        for (int y = minY; y < maxY; ++y)
        {
            for (int z = minZ; z < maxZ; ++z)
            {
                if (isSolidTile(x, y, z))
                {
                    cubes.emplace_back(x, y, z, (x + 1), (y + 1), (z + 1));
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

    if (x >= 0 && y >= 0 && z >= 0 && x < this->width && y < this->depth && z < this->height)
    {
        return (y < this->lightDepths[x + z * this->width]) ? darkBrightness : brightBrightness;
    }
    return brightBrightness;
}

void Level::setTile(int x, int y, int z, int tileId)
{
    if (x >= 0 && y >= 0 && z >= 0 && x < this->width && y < this->depth && z < this->height)
    {

        this->blocks[(y * this->height + z) * this->width + x] = tileId;
        calcLightDepths(x, z, 1, 1);

        for (int i = 0; i < this->levelListeners.size(); ++i)
        {
            this->levelListeners.at(i)->tileChanged(x, y, z);
        }
    }
}

#include "Level.h"

#include <algorithm>
#include <cmath>
#include <zlib.h>
#include <functional>
#include <fstream>
#include <vector>
#include "Level/tile/Tile.h"
#include "Level/LevelGen.h"

Level::Level(int width, int height, int depth) : width(0), height(0), depth(0), blocks(0), lightDepths(), random(rd())
{

    this->width = width;
    this->height = height;
    this->depth = depth;
    this->blocks.resize(width * height * depth);
    this->lightDepths.resize(width * height * depth);
    bool loaded = this->load();

    if (!loaded)
    {
        this->blocks = (new LevelGen(width, height, depth))->generateMap();
    }

    this->calcLightDepths(0, 0, width, height);
}

bool Level::load()
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
        return true;
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error loading level: " << ex.what() << std::endl;
        return false;
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

bool Level::isLightBlocker(int x, int y, int z)
{
    Tile *tile = Tile::tiles[getTile(x, y, z)];
    return tile == nullptr ? false : tile->blocksLight();
}


std::vector<AABB> Level::getCubes(const AABB &aabb)
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
                Tile *tile = Tile::tiles[this->getTile(x, y, z)];
                if (tile != nullptr)
                {
                    AABB *aabb = tile->getBoundingBox(x, y, z);
                    if (aabb != nullptr)
                    {
                        cubes.emplace_back(*aabb);
                    }
                }
            }
        }
    }

    return cubes;
}

bool Level::setTile(int x, int y, int z, int tileId)
{
    if (x >= 0 && y >= 0 && z >= 0 && x < this->width && y < this->depth && z < this->height)
    {
        if (tileId == this->blocks[(y * this->height + z) * this->width + x])
        {
            return false;
        }
        else
        {
            this->blocks[(y * this->height + z) * this->width + x] = tileId;
            this->calcLightDepths(x, z, 1, 1);

            for (int var5 = 0; var5 < this->levelListeners.size(); ++var5)
            {
                this->levelListeners.at(var5)->tileChanged(x, y, z);
            }

            return true;
        }
    }
    else
    {
        return false;
    }
}

bool Level::isLit(int x, int y, int z)
{
    return x >= 0 && y >= 0 && z >= 0 && x < this->width && y < this->depth && z < this->height ? y >= this->lightDepths[x + z * this->width] : true;
}

int Level::getTile(int x, int y, int z)
{
    return x >= 0 && y >= 0 && z >= 0 && x < this->width && y < this->depth && z < this->height ? this->blocks[(y * this->height + z) * this->width + x] : 0;
}

bool Level::isSolidTile(int x, int y, int z)
{
    Tile *tile = Tile::tiles[this->getTile(x, y, z)];
    return tile == nullptr ? false : tile->isSolid();
}

void Level::tick()
{
    this->unprocessed += this->width * this->height * this->depth;
    int var1 = this->unprocessed / 400;
    this->unprocessed -= var1 * 400;

    std::uniform_int_distribution<int> widthDist(0, this->width - 1);
    std::uniform_int_distribution<int> depthDist(0, this->depth - 1);
    std::uniform_int_distribution<int> heightDist(0, this->height - 1);

    for (int var2 = 0; var2 < var1; ++var2)
    {
        int var3 = widthDist(this->random);
        int var4 = depthDist(this->random);
        int var5 = heightDist(this->random);

        Tile *var6 = Tile::tiles[this->getTile(var3, var4, var5)];
        if (var6 != nullptr)
        {

            std::default_random_engine defaultRandom(this->random());
            var6->tick(this, var3, var4, var5, defaultRandom);
        }
    }
}
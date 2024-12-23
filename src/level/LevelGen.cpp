// LevelGen.cpp
#include "level/LevelGen.h"
#include "level/tile/Tile.h"
#include "utils/Math.h"
#include <cmath>
#include <algorithm>

LevelGen::LevelGen(int width, int height, int depth)
    : width(width), height(height), depth(depth), random(std::random_device{}()) {}

std::vector<unsigned int> LevelGen::generateMap()
{
    std::vector<int> noise1 = PerlinNoiseFilter(0).read(width, height);
    std::vector<int> noise2 = PerlinNoiseFilter(0).read(width, height);
    std::vector<int> noise3 = PerlinNoiseFilter(1).read(width, height);
    std::vector<int> noise4 = PerlinNoiseFilter(1).read(width, height);
    std::vector<unsigned int> map(width * height * depth, 0);

    for (int x = 0; x < width; ++x)
    {
        for (int z = 0; z < depth; ++z)
        {
            for (int h = 0; h < height; ++h)
            {
                int var12 = noise1[x + h * width];
                int var13 = noise2[x + h * width];
                int y = noise3[x + h * width];

                if (y < 128)
                {
                    var13 = var12;
                }

                int var15 = std::max(var12, var13) / 8 + depth / 3;
                int var16 = noise4[x + h * width] / 8 + depth / 3;
                var16 = std::min(var16, var15 - 2);

                int index = (z * height + h) * width + x;
                unsigned int tileId = 0;

                if (z == var15)
                {
                    tileId = Tile::grass->id;
                }
                else if (z < var15)
                {
                    tileId = Tile::dirt->id;
                }
                if (z <= var16)
                {
                    tileId = Tile::rock->id;
                }

                map[index] = tileId;
            }
        }
    }

    int numCaves = width * height * depth / 256 / 64;
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    for (int i = 0; i < numCaves; ++i)
    {
        float var29 = dist(random) * width;
        float var30 = dist(random) * depth;
        float var31 = dist(random) * height;
        int y = static_cast<int>(dist(random) + dist(random) * 150.0f);
        float var32 = dist(random) * static_cast<float>(Math::PI) * 2.0f;
        float var33 = 0.0f;
        float var34 = dist(random) * static_cast<float>(Math::PI) * 2.0f;
        float var35 = 0.0f;

        for (int var19 = 0; var19 < y; ++var19)
        {
            var29 += std::sin(var32) * std::cos(var34);
            var31 += std::cos(var32) * std::cos(var34);
            var30 += std::sin(var34);
            var32 += var33 * 0.2f;
            var33 *= 0.9f;
            var33 += dist(random) - dist(random);
            var34 += var35 * 0.5f;
            var35 *= 0.9f;
            var35 += dist(random) - dist(random);
            float var20 = std::sin(static_cast<double>(var19) * Math::PI / y) * 2.5f + 1.0f;

            for (int var21 = static_cast<int>(var29 - var20); var21 <= static_cast<int>(var29 + var20); ++var21)
            {
                for (int var22 = static_cast<int>(var30 - var20); var22 <= static_cast<int>(var30 + var20); ++var22)
                {
                    for (int var23 = static_cast<int>(var31 - var20); var23 <= static_cast<int>(var31 + var20); ++var23)
                    {
                        float var24 = var21 - var29;
                        float var25 = var22 - var30;
                        float var26 = var23 - var31;
                        float var27 = var24 * var24 + var25 * var25 * 2.0f + var26 * var26;

                        if (var27 < var20 * var20 &&
                            var21 >= 1 && var22 >= 1 && var23 >= 1 &&
                            var21 < width - 1 && var22 < depth - 1 && var23 < height - 1)
                        {
                            int var28 = (var22 * height + var23) * width + var21;
                            if (map[var28] == Tile::rock->id)
                            {
                                map[var28] = 0;
                            }
                        }
                    }
                }
            }
        }
    }

    return map;
}
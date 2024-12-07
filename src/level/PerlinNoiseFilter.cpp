#include "PerlinNoiseFilter.h"

PerlinNoiseFilter::PerlinNoiseFilter(int levels)
    : levels(levels), fuzz(16)
{
    std::random_device rd;
    random = std::mt19937(rd());
    seed = random();
}

std::vector<int> PerlinNoiseFilter::read(int width, int height)
{
    std::mt19937 var3(seed);
    std::vector<int> data(width * height, 0);
    int var5 = this->levels;
    int var6 = width >> var5;

    for (int y = 0; y < height; y += var6)
    {
        for (int x = 0; x < width; x += var6)
        {
            data[x + y * width] = (var3() % 256 - 128) * this->fuzz;
        }
    }

    for (var6 = width >> var5; var6 > 1; var6 /= 2)
    {
        int var7 = 256 * (var6 << var5);
        int var8 = var6 / 2;

        for (int y = 0; y < height; y += var6)
        {
            for (int x = 0; x < width; x += var6)
            {
                int var11 = data[(x + 0) % width + (y + 0) % height * width];
                int var12 = data[(x + var6) % width + (y + 0) % height * width];
                int var13 = data[(x + 0) % width + (y + var6) % height * width];
                int var14 = data[(x + var6) % width + (y + var6) % height * width];
                int var15 = (var11 + var13 + var12 + var14) / 4 + var3() % (var7 * 2) - var7;
                data[x + var8 + (y + var8) * width] = var15;
            }
        }

        for (int y = 0; y < height; y += var6)
        {
            for (int x = 0; x < width; x += var6)
            {
                int var11 = data[x + y * width];
                int var12 = data[(x + var6) % width + y * width];
                int var13 = data[x + (y + var6) % height * width];
                int var14 = data[(x + var8 & width - 1) + (y + var8 - var6 & height - 1) * width];
                int var15 = data[(x + var8 - var6 & width - 1) + (y + var8 & height - 1) * width];
                int var16 = data[(x + var8) % width + (y + var8) % height * width];
                int var17 = (var11 + var12 + var16 + var14) / 4 + var3() % (var7 * 2) - var7;
                int var18 = (var11 + var13 + var16 + var15) / 4 + var3() % (var7 * 2) - var7;
                data[x + var8 + y * width] = var17;
                data[x + (y + var8) * width] = var18;
            }
        }
    }

    std::vector<int> result(width * height);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            result[x + y * width] = data[x % width + y % height * width] / 512 + 128;
        }
    }

    return result;
}

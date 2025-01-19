#pragma once
#include <vector>
#include <random>

class NoiseMap
{
private:
    std::mt19937 random;
    int levels;
    int fuzz;
    bool flag;

public:
    NoiseMap(std::mt19937 random, int levels, bool flag);
    std::vector<int> read(int width, int height);
};
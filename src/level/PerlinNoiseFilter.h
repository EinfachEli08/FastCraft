#pragma once
#include <vector>
#include <random>

class PerlinNoiseFilter
{
private:
    std::mt19937 random;
    int seed;
    int levels;
    int fuzz;

public:
    explicit PerlinNoiseFilter(int levels);

    std::vector<int> read(int width, int height);
};

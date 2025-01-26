#pragma once

#include "Player.h"
#include "Chunk.h"
#include <functional>

class DistanceSorter
{
private:
    Player *thePlayer;

public:
    DistanceSorter(Player *player);
    int compare(Chunk *var1, Chunk *var2) const;
    bool operator()(Chunk *var1, Chunk *var2) const;
};
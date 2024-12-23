#pragma once

#include <chrono>
#include <functional>
#include "Player.h"
#include "renderer/Frustum.h"
#include "Chunk.h"

class DirtyChunkSorter
{
private:
    Player *player;
    Frustum *frustum;
    long long now;

public:
    DirtyChunkSorter(Player *player, Frustum *frustum);
    int compare(Chunk *var1, Chunk *var2);
    bool operator()(Chunk *var1, Chunk *var2);
};

#pragma once
#include "Player.h"

class HitResult
{
public:
    int x;
    int y;
    int z;
    int f;

    HitResult(int type, int x, int y, int z, int f);

    float distanceTo(Player *player, int var2);	
        
};

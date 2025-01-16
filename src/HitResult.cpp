#include "HitResult.h"

HitResult::HitResult(int type, int x, int y, int z, int f)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->f = f;
}

float HitResult::distanceTo(Player *player, int var2){
    int x = this->x;
    int y = this->y;
    int z = this->z;

    if(var2 == 1){
        if(this->f == 0){
            --y;
        }
        if (this->f == 1)
        {
            --y;
        }
        if (this->f == 2)
        {
            --z;
        }
        if (this->f == 3)
        {
            --z;
        }
        if (this->f == 4)
        {
            --x;
        }
        if (this->f == 5)
        {
            --x;
        }
       
    }

    float var6 = (float)x - player->x;
    float var8 = (float)y - player->y;
    float var7 = (float)z - player->z;
    return var6 * var6 + var8 * var8 + var7 * var7;
}
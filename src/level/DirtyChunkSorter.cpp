#include "DirtyChunkSorter.h"
#include <cmath>

DirtyChunkSorter::DirtyChunkSorter(Player *player, Frustum *frustum)
    : player(player), frustum(frustum)
{
   
    now = std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::system_clock::now().time_since_epoch())
              .count();
}

int DirtyChunkSorter::compare(Chunk *var1, Chunk *var2)
{
    bool var3 = frustum->isVisible(&(var1->aabb));
    bool var4 = frustum->isVisible(&(var2->aabb));

    if (var3 && !var4)
    {
        return -1;
    }
    else if (var4 && !var3)
    {
        return 1;
    }
    else
    {
        int var5 = static_cast<int>((now - var1->dirtiedTime) / 2000L);
        int var6 = static_cast<int>((now - var2->dirtiedTime) / 2000L);
        if (var5 < var6)
        {
            return -1;
        }
        else if (var5 > var6)
        {
            return 1;
        }
        else
        {
            return (var1->distanceToSqr(player) < var2->distanceToSqr(player)) ? -1 : 1;
        }
    }
}

bool DirtyChunkSorter::operator()(Chunk *var1, Chunk *var2)
{
    return compare(var1, var2) < 0;
}

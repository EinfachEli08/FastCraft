#include "DistanceSorter.h"

DistanceSorter::DistanceSorter(Player *player) : thePlayer(player) {}

int DistanceSorter::compare(Chunk *var1, Chunk *var2) const
{
    return var1->distanceToSqr(this->thePlayer) < var2->distanceToSqr(this->thePlayer) ? -1 : 1;
}

bool DistanceSorter::operator()(Chunk *var1, Chunk *var2) const
{
    return compare(var1, var2) < 0;
}
#include "Fastcraft.h"

int main()
{
    Fastcraft *fastcraft = new Fastcraft(854, 480, false);
    fastcraft->appletMode = true;
    fastcraft->pause = false;
    fastcraft->run();

    return 0;
}

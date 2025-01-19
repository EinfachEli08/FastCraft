#pragma once

#include "Entity.h"
#include "utils/Controller.h"
#include <vector>

class Player : public Entity
{
public:
    // Constructor
    Player(Level *level);

    // Methods
    void tick();
};


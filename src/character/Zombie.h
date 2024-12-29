#pragma once

#include "Entity.h"
#include "character/Cube.h"
#include "utils/Math.h"
#include "character/ZombieModel.h"
#include "renderer/Textures.h"
#include <cmath>
#include <cstdlib>

class Zombie : public Entity
{
private:
    static ZombieModel zombieModel;
    Textures *textures;

public:
    Cube head;
    Cube body;
    Cube arm0;
    Cube arm1;
    Cube leg0;
    Cube leg1;
    float rot;
    float timeOffs;
    float speed;
    float rotA;

    Zombie(Level *level, Textures *textures, float x, float y, float z);

    void tick() override;
    void render(float partialTick) override;
};

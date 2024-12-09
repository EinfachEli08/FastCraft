#ifndef PARTICLE_H
#define PARTICLE_H

#include "Entity.h"
#include "level/Level.h"
#include "level/Tesselator.h"

class Particle : public Entity
{
public:
    float xd, yd, zd;
    int tex;
    float uo, vo;

    Particle(Level *level, float x, float y, float z, float xd, float yd, float zd, int tex);
    void tick();
    void render(Tesselator &tesselator, float deltaTime, float var2, float var3, float var4);

private:
    float xo, yo, zo;
};

#endif // PARTICLE_H

#ifndef PARTICLE_ENGINE_H
#define PARTICLE_ENGINE_H

#include <vector>
#include "Particle.h"
#include "Player.h"
#include "renderer/Textures.h"
#include "level/Level.h"
#include "renderer/Tesselator.h"

class ParticleEngine
{
public:
    ParticleEngine(Level *level);
    void add(Particle *particle);
    void tick();
    void render(Player *player, float deltaTime, int renderMode);

private:
    Level *level;
    std::vector<Particle *> particles;
};

#endif // PARTICLE_ENGINE_H

#include "ParticleEngine.h"
#include <GL/gl.h>

ParticleEngine::ParticleEngine(Level *level) : level(level) {}

void ParticleEngine::add(Particle *particle)
{
    particles.push_back(particle);
}

void ParticleEngine::tick()
{
    for (size_t i = 0; i < particles.size(); ++i)
    {
        Particle *particle = particles[i];
        particle->tick();
        if (particle->removed)
        {
            particles.erase(particles.begin() + i--);
        }
    }
}

void ParticleEngine::render(Player *player, float deltaTime, int renderMode)
{
    glEnable(GL_TEXTURE_2D);
    int textureID = Textures::loadTexture("assets/terrain.png", 9728);
    glBindTexture(GL_TEXTURE_2D, textureID);

    float var5 = -(float)std::cos(player->yRot * Math::PI / 180.0f);
    float var6 = -(float)std::sin(player->yRot * Math::PI / 180.0f);
    float var7 = 1.0f;
    Tesselator &tesselator = Tesselator::getInstance();
    glColor4f(0.8f, 0.8f, 0.8f, 1.0f);
    tesselator.init();

    for (size_t i = 0; i < particles.size(); ++i)
    {
        Particle *particle = particles[i];
        if (particle->isLit() ^ renderMode == 1)
        {
            particle->render(tesselator, deltaTime, var5, var7, var6);
        }
    }

    tesselator.flush();
    glDisable(GL_TEXTURE_2D);
}

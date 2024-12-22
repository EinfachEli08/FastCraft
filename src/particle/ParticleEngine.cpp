#include "ParticleEngine.h"
#include <GL/gl.h>

ParticleEngine::ParticleEngine(Level *level) : level(level) {}

void ParticleEngine::add(Particle *particle)
{
    this->particles.push_back(particle);
}

void ParticleEngine::tick()
{
    for (int i = 0; i < this->particles.size(); ++i)
    {
        Particle *particle = this->particles[i];
        particle->tick();
        if (particle->removed)
        {
            this->particles.erase(particles.begin() + i--);
        }
    }
}

void ParticleEngine::render(Player *player, float deltaTime, int renderMode)
{
    glEnable(GL_TEXTURE_2D);
    int textureID = Textures::loadTexture("assets/terrain.png", 9728);
    glBindTexture(GL_TEXTURE_2D, textureID);

    float var5 = -((float)std::cos((double)player->yRot * Math::PI / 180.0f));
    float var6 = -((float)std::sin((double)player->yRot * Math::PI / 180.0f));

    float var7 = -var6 * (float)std::sin((double)player->xRot * Math::PI / 180.0D);
    float var8 = var5 * (float)std::sin((double)player->xRot * Math::PI / 180.0D);
    float var9 = (float)std::cos((double)player->xRot * Math::PI / 180.0D);

    Tesselator &tesselator = Tesselator::getInstance();
    glColor4f(0.8f, 0.8f, 0.8f, 1.0f);
    tesselator.init();

    for (int i = 0; i < this->particles.size(); ++i)
    {
        Particle *particle = this->particles[i];
        if (particle->isLit() ^ renderMode == 1)
        {
            particle->render(tesselator, deltaTime, var5, var9, var6, var7, var8);
        }
    }

    tesselator.flush();
    glDisable(GL_TEXTURE_2D);
}

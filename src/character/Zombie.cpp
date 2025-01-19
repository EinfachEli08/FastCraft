#include "Zombie.h"
#include <renderer/Textures.h>
#include <chrono>
#include <glad/glad.h>
#include "renderer/Textures.h"

Zombie::Zombie(Level *level, Textures *textures, float x, float y, float z)
    : Entity(level),
      textures(textures),
      head(0, 0), body(16, 16), arm0(40, 16), arm1(40, 16), leg0(0, 16), leg1(0, 16)
{
    this->textures = textures;
    this->rotA = (float)(Math::random() + 1.0D) * 0.01F;
    this->setPos(x, y, z);
    this->timeOffs = (float)(Math::random()) * 1239813.0f;
    this->rot = (float)(Math::random()) * (float)(Math::PI * 2.0);
    this->speed = 1.0f;
}

void Zombie::tick()
{
    this->xo = this->x;
    this->yo = this->y;
    this->zo = this->z;

    float var1 = 0.0F;
    float var2 = 0.0F;

    if (this->y < -100.0F)
    {
        this->remove();
    }

    this->rot += this->rotA;
    this->rotA = (float)((double)this->rotA * 0.99D);
    this->rotA = (float)((double)this->rotA + (Math::random() - Math::random()) * Math::random() * Math::random() * 0.08F);

    var1 = (float)std::sin(this->rot);
    var2 = (float)std::cos(this->rot);

    if (this->onGround && Math::random() < 0.08D){
        this->yd = 0.5F;
    }

    this->moveRelative(var1, var2, this->onGround ? 0.1F : 0.02F);
    this->yd = this->yd - 0.08D;
    this->move(this->xd, this->yd, this->zd);

    this->xd *= 0.91f;
    this->yd *= 0.98f;
    this->zd *= 0.91f;

    if (this->onGround){
        this->xd *= 0.7f;
        this->zd *= 0.7f;
    }
}

void Zombie::render(float partialTick)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, this->textures->loadTexture("assets/char.png", GL_NEAREST));
    glPushMatrix();

    double var2 = static_cast<double>(std::chrono::high_resolution_clock::now().time_since_epoch().count()) / 1.0E9 * 10.0 * static_cast<double>(this->speed) + static_cast<double>(this->timeOffs);
    float var4 = 0.058333334f;
    float var5 = static_cast<float>(-std::abs(std::sin(var2 * 0.6662)) * 5.0 - 23.0);

    glTranslatef(this->xo + (this->x - this->xo) * partialTick, this->yo + (this->y - this->yo) * partialTick, this->zo + (this->z - this->zo) * partialTick);
    glScalef(1.0F, -1.0F, 1.0F);
    glScalef(var4, var4, var4);
    glTranslatef(0.0F, var5, 0.0F);
    glRotatef(this->rot * 57.29578f + 180.0f, 0.0f, 1.0f, 0.0f);

    ZombieModel zombieModel = ZombieModel();
    zombieModel.head.yRot = (float)std::sin((double)var2 * 0.83D);
    zombieModel.head.xRot = (float)std::sin((double)var2) * 0.8f;

    zombieModel.arm0.xRot = (float)std::sin((double)var2 * 0.6662D + Math::PI) * 2.0f;
    zombieModel.arm0.zRot = (float)(std::sin((double)var2 * 0.2312D)) + 1.0D;

    zombieModel.arm1.xRot = (float)std::sin((double)var2 * 0.6662D) * 2.0f;
    zombieModel.arm1.zRot = (float)(std::sin((double)var2 * 0.2812D) - 1.0D);

    zombieModel.leg0.xRot = (float)std::sin((double)var2 * 0.6662D) * 1.4f;
    zombieModel.leg1.xRot = (float)std::sin((double)var2 * 0.6662D + Math::PI) * 1.4f;

    zombieModel.head.render();
    zombieModel.body.render();
    zombieModel.arm0.render();
    zombieModel.arm1.render();
    zombieModel.leg0.render();
    zombieModel.leg1.render();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
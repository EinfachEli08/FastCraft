
#include "Zombie.h"

#include <Textures.h>
#include <chrono>
#include <glad/glad.h>

ZombieModel Zombie::zombieModel;

Zombie::Zombie(Level *level, float x, float y, float z)
    : Entity(level),
      head(0, 0), body(16, 16), arm0(40, 16), arm1(40, 16), leg0(0, 16), leg1(0, 16)
{
    this->setPos(x, y, z);

    this->timeOffs = static_cast<float>(std::rand()) * 1239813.0f;
    this->rot = static_cast<float>(std::rand()) * static_cast<float>(Math::PI * 2.0);
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
    this->rotA = this->rotA * 0.99D;
    this->rotA = (this->rotA + (Math::random() - Math::random()) * Math::random() * Math::random() * 0.08F);

    var1 = std::sin(this->rot);
    var2 = std::cos(this->rot);

    if (this->onGround && Math::random() < 0.08D)
    {
        this->yd = 0.5F;
    }

    this->moveRelative(var1, var2, this->onGround ? 0.1F : 0.02F);
    this->yd = this->yd - 0.08D;
    this->move(this->xd, this->yd, this->zd);

    this->xd *= 0.91f;
    this->yd *= 0.98f;
    this->zd *= 0.91f;

    if (this->onGround)
    {
        this->xd *= 0.7f;
        this->zd *= 0.7f;
    }
}

void Zombie::render(float partialTick)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Textures::loadTexture("assets/char.png", GL_NEAREST));
    glPushMatrix();

    double var2 = static_cast<double>(std::chrono::system_clock::now().time_since_epoch().count()) / 1.0e9D * 10.0D * this->speed + this->timeOffs;
    float var4 = 0.058333334f;
    float var5 = static_cast<float>(-std::abs(std::sin(var2 * 0.6662)) * 5.0 - 23.0);

    glTranslatef(this->xo + (this->x - this->xo) * partialTick, this->yo + (this->y - this->yo) * partialTick, this->zo + (this->z - this->zo) * partialTick);
    glScalef(1.0f, -1.0f, 1.0f);
    glScalef(var4, var4, var4);
    glTranslatef(0.0f, var5, 0.0f);

    glRotatef(this->rot * 57.29578f + 180.0f, 0.0f, 1.0f, 0.0f);

    zombieModel.render(var2);

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

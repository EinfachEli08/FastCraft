#include "Zombie.h"
#include <glad/glad.h> // Or glad if you're using it
#include <Textures.h>
#include <chrono>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Zombie::Zombie(Level *level, float x, float y, float z)
    : Entity(level),
      head(0, 0), body(16, 16), arm0(40, 16), arm1(40, 16), leg0(0, 16), leg1(0, 16)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->timeOffs = static_cast<float>(std::rand()) * 1239813.0f;
    this->rot = static_cast<float>(std::rand()) * static_cast<float>(M_PI * 2.0);
    this->speed = 1.0f;

    this->head.addBox(-4.0f, -8.0f, -4.0f, 8, 8, 8);
    this->body.addBox(-4.0f, 0.0f, -2.0f, 8, 12, 4);
    this->arm0.addBox(-3.0f, -2.0f, -2.0f, 4, 12, 4);
    this->arm0.setPos(-5.0f, 2.0f, 0.0f);
    this->arm1.addBox(-1.0f, -2.0f, -2.0f, 4, 12, 4);
    this->arm1.setPos(5.0f, 2.0f, 0.0f);
    this->leg0.addBox(-2.0f, 0.0f, -2.0f, 4, 12, 4);
    this->leg0.setPos(-2.0f, 12.0f, 0.0f);
    this->leg1.addBox(-2.0f, 0.0f, -2.0f, 4, 12, 4);
    this->leg1.setPos(2.0f, 12.0f, 0.0f);
}

void Zombie::tick()
{
    this->xo = this->x;
    this->yo = this->y;
    this->zo = this->z;

    float var1 = 0.0F;
    float var2 = 0.0F;

    this->rot += this->rotA;
    this->rotA = this->rotA * 0.99D;
    this->rotA = (this->rotA + (Math::random() - Math::random()) * Math::random() * Math::random() * 0.01F);

    var1 = std::sin(this->rot);
    var2 = std::cos(this->rot);

    if (this->onGround && Math::random() < 0.01D)
    {
        this->yd = 0.12F;
    }

    this->moveRelative(var1, var2, this->onGround ? 0.02f : 0.005f);
    this->yd -= 0.005f;
    this->move(this->xd, this->yd, this->zd);

    this->xd *= 0.91f;
    this->yd *= 0.98f;
    this->zd *= 0.91f;

    if (this->y > 100.0f)
    {
        this->resetPos();
    }

    if (this->onGround)
    {
        this->xd *= 0.8f;
        this->zd *= 0.8f;
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

    this->head.yRot = static_cast<float>(std::sin(var2 * 0.83)) * 1.0f;
    this->head.xRot = static_cast<float>(std::sin(var2)) * 0.8f;

    this->arm0.xRot = static_cast<float>(std::sin(var2 * 0.6662 + M_PI)) * 2.0f;
    this->arm0.zRot = static_cast<float>(std::sin(var2 * 0.2312) + 1.0) * 1.0f;

    this->arm1.xRot = static_cast<float>(std::sin(var2 * 0.6662)) * 2.0f;
    this->arm1.zRot = static_cast<float>(std::sin(var2 * 0.2812) - 1.0) * 1.0f;

    this->leg0.xRot = static_cast<float>(std::sin(var2 * 0.6662)) * 1.4f;
    this->leg1.xRot = static_cast<float>(std::sin(var2 * 0.6662 + M_PI)) * 1.4f;

    this->head.render();
    this->body.render();
    this->arm0.render();
    this->arm1.render();
    this->leg0.render();
    this->leg1.render();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

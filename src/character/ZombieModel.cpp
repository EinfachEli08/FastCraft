#include "ZombieModel.h"

ZombieModel::ZombieModel() : head(0, 0), body(16, 16), arm0(40, 16), arm1(40, 16), leg0(0, 16), leg1(0, 16)
{
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

void ZombieModel::render(double var2)
{
    this->head.yRot = static_cast<float>(std::sin(var2 * 0.83)) * 1.0f;
    this->head.xRot = static_cast<float>(std::sin(var2)) * 0.8f;

    this->arm0.xRot = static_cast<float>(std::sin(var2 * 0.6662 + Math::PI)) * 2.0f;
    this->arm0.zRot = static_cast<float>(std::sin(var2 * 0.2312) + 1.0) * 1.0f;

    this->arm1.xRot = static_cast<float>(std::sin(var2 * 0.6662)) * 2.0f;
    this->arm1.zRot = static_cast<float>(std::sin(var2 * 0.2812) - 1.0) * 1.0f;

    this->leg0.xRot = static_cast<float>(std::sin(var2 * 0.6662)) * 1.4f;
    this->leg1.xRot = static_cast<float>(std::sin(var2 * 0.6662 + Math::PI)) * 1.4f;

    this->head.render();
    this->body.render();
    this->arm0.render();
    this->arm1.render();
    this->leg0.render();
    this->leg1.render();

}

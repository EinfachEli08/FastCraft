#include "Vec3.h"

Vec3::Vec3(float var1, float var2, float var3){
    this->x = var1;
    this->y = var2;
    this->z = var3;
}

Vec3 Vec3::interpolateTo(Vec3 &var1, float var2)
{
    float var3 = this->x + (var1.x - this->x) * var2;
    float var4 = this->y + (var1.y - this->y) * var2;
    float var5 = this->z + (var1.z - this->z) * var2;
    return Vec3(var3, var4, var5);
}

void Vec3::set(float var1, float var2, float var3)
{
    this->x = var1;
    this->y = var2;
    this->z = var3;
}

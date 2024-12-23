#pragma once 

class Vec3
{
public:
    float x, y, z;

    Vec3(float var1, float var2, float var3);

    Vec3 interpolateTo(Vec3 &var1, float var2);

    void set(float var1, float var2, float var3);
};

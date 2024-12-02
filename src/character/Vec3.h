#pragma once 

class Vec3
{
public:
    float x, y, z;

    Vec3(float var1, float var2, float var3);

    Vec3 interpolateTo(const Vec3 &var1, float var2) const;

    void set(float var1, float var2, float var3);
};

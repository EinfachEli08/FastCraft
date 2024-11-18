#pragma once

class AABB
{
private:
    float epsilon;

public:
    float x0, y0, z0, x1, y1, z1;

    // Default constructor
    AABB() : x0(0), y0(0), z0(0), x1(0), y1(0), z1(0) {}

    // Constructor with parameters
    AABB(float x0, float y0, float z0, float x1, float y1, float z1)
        : x0(x0), y0(y0), z0(z0), x1(x1), y1(y1), z1(z1) {}

    AABB expand(float var1, float var2, float var3) const;
    AABB grow(float var1, float var2, float var3) const;
    float clipXCollide(const AABB &var1, float var2) const;
    float clipYCollide(const AABB &var1, float var2) const;
    float clipZCollide(const AABB &var1, float var2) const;
    bool intersects(const AABB &var1) const;
    void move(float var1, float var2, float var3);
};

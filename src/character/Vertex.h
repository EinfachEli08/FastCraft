#pragma once
#include "Vec3.h"

class Vertex
{
public:
    float x, y, z, u, v;

    Vertex();                                                           
    Vertex(float var1, float var2, float var3, float var4, float var5); 
    Vertex(const Vertex &var1, float var2, float var3);                
    Vertex(const Vec3 &var1, float var2, float var3);                   
    Vertex remap(float var1, float var2) const;                         
};

#include "Vertex.h"

Vertex::Vertex() : x(0.0f), y(0.0f), z(0.0f), u(0.0f), v(0.0f) {}

Vertex::Vertex(float var1, float var2, float var3, float var4, float var5)
    : x(var1), y(var2), z(var3), u(var4), v(var5) {}

Vertex::Vertex(const Vertex &var1, float var2, float var3)
    : x(var1.x), y(var1.y), z(var1.z), u(var2), v(var3) {}

Vertex::Vertex(const Vec3 &var1, float var2, float var3)
    : x(var1.x), y(var1.y), z(var1.z), u(var2), v(var3) {}
                                    
Vertex Vertex::remap(float var1, float var2) const
{
    return Vertex(*this, var1, var2);
}

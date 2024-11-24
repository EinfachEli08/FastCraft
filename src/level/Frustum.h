#include <GLFW/glfw3.h>
#include <cmath>
#include <array>

class Frustum
{
public:
    static const int RIGHT = 0;
    static const int LEFT = 1;
    static const int BOTTOM = 2;
    static const int TOP = 3;
    static const int BACK = 4;
    static const int FRONT = 5;

    static const int A = 0;
    static const int B = 1;
    static const int C = 2;
    static const int D = 3;

    static Frustum &getInstance();

    void calculateFrustum();
    bool pointInFrustum(float x, float y, float z) const;
    bool sphereInFrustum(float x, float y, float z, float radius) const;
    bool cubeFullyInFrustum(float x0, float y0, float z0, float x1, float y1, float z1) const;
    bool cubeInFrustum(float x0, float y0, float z0, float x1, float y1, float z1) const;

private:
    Frustum() = default;

    void normalizePlane(std::array<std::array<float, 4>, 6> &frustum, int side);

    std::array<std::array<float, 4>, 6> m_Frustum;
    std::array<float, 16> proj;
    std::array<float, 16> modl;
    std::array<float, 16> clip;
};
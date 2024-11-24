#include "Frustum.h"

Frustum &Frustum::getInstance()
{
    static Frustum instance;
    instance.calculateFrustum();
    return instance;
}

void Frustum::normalizePlane(std::array<std::array<float, 4>, 6> &frustum, int side)
{
    float magnitude = std::sqrt(frustum[side][0] * frustum[side][0] +
                                frustum[side][1] * frustum[side][1] +
                                frustum[side][2] * frustum[side][2]);

    
    frustum[side][0] /= magnitude;
    frustum[side][1] /= magnitude;
    frustum[side][2] /= magnitude;
    frustum[side][3] /= magnitude;
}

void Frustum::calculateFrustum()
{
    float projection[16];
    float modelview[16];
    float clipMatrix[16];

    glGetFloatv(GL_PROJECTION_MATRIX, projection);
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

    // Multiply modelview and projection matrices to get clip matrix
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            clipMatrix[i * 4 + j] = modelview[i * 4 + 0] * projection[0 * 4 + j] +
                                    modelview[i * 4 + 1] * projection[1 * 4 + j] +
                                    modelview[i * 4 + 2] * projection[2 * 4 + j] +
                                    modelview[i * 4 + 3] * projection[3 * 4 + j];
        }
    }

    // Extract planes
    for (int i = 0; i < 6; ++i)
    {
        int sign = (i % 2 == 0) ? -1 : 1;
        int planeIdx = i / 2;

        m_Frustum[i][0] = clipMatrix[3] + sign * clipMatrix[planeIdx];
        m_Frustum[i][1] = clipMatrix[7] + sign * clipMatrix[planeIdx + 4];
        m_Frustum[i][2] = clipMatrix[11] + sign * clipMatrix[planeIdx + 8];
        m_Frustum[i][3] = clipMatrix[15] + sign * clipMatrix[planeIdx + 12];

        normalizePlane(m_Frustum, i);
    }
}

bool Frustum::pointInFrustum(float x, float y, float z) const
{
    for (int i = 0; i < 6; ++i)
    {
        if (m_Frustum[i][0] * x + m_Frustum[i][1] * y + m_Frustum[i][2] * z + m_Frustum[i][3] <= 0)
        {
            return false;
        }
    }
    return true;
}

bool Frustum::sphereInFrustum(float x, float y, float z, float radius) const
{
    for (int i = 0; i < 6; ++i)
    {
        if (m_Frustum[i][0] * x + m_Frustum[i][1] * y + m_Frustum[i][2] * z + m_Frustum[i][3] <= -radius)
        {
            return false;
        }
    }
    return true;
}

bool Frustum::cubeFullyInFrustum(float x0, float y0, float z0, float x1, float y1, float z1) const
{
    for (int i = 0; i < 6; ++i)
    {
        if (m_Frustum[i][0] * x0 + m_Frustum[i][1] * y0 + m_Frustum[i][2] * z0 + m_Frustum[i][3] <= 0 &&
            m_Frustum[i][0] * x1 + m_Frustum[i][1] * y0 + m_Frustum[i][2] * z0 + m_Frustum[i][3] <= 0 &&
            m_Frustum[i][0] * x0 + m_Frustum[i][1] * y1 + m_Frustum[i][2] * z0 + m_Frustum[i][3] <= 0 &&
            m_Frustum[i][0] * x1 + m_Frustum[i][1] * y1 + m_Frustum[i][2] * z0 + m_Frustum[i][3] <= 0 &&
            m_Frustum[i][0] * x0 + m_Frustum[i][1] * y0 + m_Frustum[i][2] * z1 + m_Frustum[i][3] <= 0 &&
            m_Frustum[i][0] * x1 + m_Frustum[i][1] * y0 + m_Frustum[i][2] * z1 + m_Frustum[i][3] <= 0 &&
            m_Frustum[i][0] * x0 + m_Frustum[i][1] * y1 + m_Frustum[i][2] * z1 + m_Frustum[i][3] <= 0 &&
            m_Frustum[i][0] * x1 + m_Frustum[i][1] * y1 + m_Frustum[i][2] * z1 + m_Frustum[i][3] <= 0)
        {
            return false;
        }
    }
    return true;
}

bool Frustum::cubeInFrustum(float x0, float y0, float z0, float x1, float y1, float z1) const
{
    for (int i = 0; i < 6; ++i)
    {
        if (m_Frustum[i][0] * x0 + m_Frustum[i][1] * y0 + m_Frustum[i][2] * z0 + m_Frustum[i][3] > 0 ||
            m_Frustum[i][0] * x1 + m_Frustum[i][1] * y0 + m_Frustum[i][2] * z0 + m_Frustum[i][3] > 0 ||
            m_Frustum[i][0] * x0 + m_Frustum[i][1] * y1 + m_Frustum[i][2] * z0 + m_Frustum[i][3] > 0 ||
            m_Frustum[i][0] * x1 + m_Frustum[i][1] * y1 + m_Frustum[i][2] * z0 + m_Frustum[i][3] > 0 ||
            m_Frustum[i][0] * x0 + m_Frustum[i][1] * y0 + m_Frustum[i][2] * z1 + m_Frustum[i][3] > 0 ||
            m_Frustum[i][0] * x1 + m_Frustum[i][1] * y0 + m_Frustum[i][2] * z1 + m_Frustum[i][3] > 0 ||
            m_Frustum[i][0] * x0 + m_Frustum[i][1] * y1 + m_Frustum[i][2] * z1 + m_Frustum[i][3] > 0 ||
            m_Frustum[i][0] * x1 + m_Frustum[i][1] * y1 + m_Frustum[i][2] * z1 + m_Frustum[i][3] > 0)
        {
            return true;
        }
    }
    return false;
}

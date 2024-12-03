#pragma once
#include <vector>

class Tesselator
{
public:
    Tesselator();
    ~Tesselator();

    void init();
    void tex(float u, float v);
    void color(float r, float g, float b);
    void vertex(float x, float y, float z);
    void flush();

private:
    void clear();

    static const int MAX_VERTICES = 100000;

    std::vector<float> vertexBuffer;
    std::vector<float> texCoordBuffer;
    std::vector<float> colorBuffer;

    int vertices;

    float u, v;    // Texture coordinates
    float r, g, b; // Color
    bool hasColor;
    bool hasTexture;
};

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
    void vertexUV(float x, float y, float z, float u, float v);
    void flush();

private:
    void clear();

    static const int MAX_MEMORY_USE = 4194304; // Matches Java's 4MB buffer
    static const int MAX_FLOATS = 524288;      // Number of floats (4MB / 4 bytes per float)

    std::vector<float> buffer;
    int vertices;
    float u, v;    // Texture coordinates
    float r, g, b; // Color
    bool hasColor;
    bool hasTexture;
    int len;
    int p;
};

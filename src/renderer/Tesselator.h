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
    void color(int hex);
    void noColor();
    void vertex(float x, float y, float z);
    void vertexUV(float x, float y, float z, float u, float v);
    void flush();

    static Tesselator &getInstance();

private:
    void clear();

    static Tesselator instance;

    static const int MAX_MEMORY_USE = 4194304; 
    static const int MAX_FLOATS = 524288;     

    std::vector<float> buffer;
    std::vector<float> array;

    int vertices;
    float u, v;    
    float r, g, b;
    bool hasColor;
    bool hasTexture;
    bool hasNoColor;
    int len;
    int p;
};

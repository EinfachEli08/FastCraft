#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "Level.h"
#include "Tesselator.h"
#include "Textures.h"
#include "AABB.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

class Chunk
{
public:
    AABB aabb;
    Level *level;
    int x0, y0, z0, x1, y1, z1;
    bool dirty;
    GLuint lists;

    static GLuint texture;
    static Tesselator t;
    static int rebuiltThisFrame;
    static int updates;

    Chunk(Level *level, int x0, int y0, int z0, int x1, int y1, int z1);
    void rebuild(int var1);
    void render(int var1);
    void setDirty();

private:
    void generateChunkList();
};

#endif // CHUNK_HPP

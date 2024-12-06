#pragma once

#include <chrono>

#include "level/Level.h"
#include "level/Tesselator.h"
#include "Player.h"
#include "Textures.h"
#include "level/tile/Tile.h"

class Chunk
{
private:
    long totalTime;
    int totalUpdates;

public:
    AABB aabb;
    Level *level;
    int x0, y0, z0, x1, y1, z1;
    float x, y, z;
    long long dirtiedTime = 0L;
    bool dirty;
    GLuint lists;

    static GLuint texture;
    static Tesselator t;
    static int rebuiltThisFrame;
    static int updates;

    static void init(GLuint tex);

    Chunk(Level *level, int x0, int y0, int z0, int x1, int y1, int z1);
    void rebuild(); 
    void render(int var1);
    void setDirty();

    bool isDirty();
    float distanceToSqr(Player *player);

private:
    void rebuild(int var1);
    void generateChunkList();
};

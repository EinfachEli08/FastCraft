#pragma once

#include "level/Tesselator.h"
#include "level/Level.h"
#include "particle/ParticleEngine.h"
#include <vector>


class Tile
{
public:
    // Static instances of Tile
    static std::vector<Tile *> tiles; // Array of all tiles
    static Tile *empty;
    static Tile *rock;
    static Tile *grass;
    static Tile *dirt;
    static Tile *stoneBrick;
    static Tile *wood;

    int id;
    int tex;

    Tile(int id);
    Tile(int id, int tex);
    virtual ~Tile() = default;

    virtual void render(Tesselator &tesselator, Level *level, int x, int y, int z, int size);
    virtual void renderFace(Tesselator &tesselator, int x, int y, int z, int face);
    virtual void renderFaceNoTexture(Tesselator &tesselator, int x, int y, int z, int face); 
    virtual bool blocksLight();
    virtual bool isSolid();
    virtual void tick(Level *level, int x, int y, int z, std::default_random_engine &random);
    virtual void destroy(Level *level, int x, int y, int z, ParticleEngine &particleEngine);
    AABB* getAABB(int x, int y, int z);

protected:
    virtual int getTexture(int face);
    bool shouldRenderFace(Level *level, int x, int y, int z, int size);
};

#pragma once

#include "renderer/Tesselator.h"
#include "level/Level.h"
#include "particle/ParticleEngine.h"
#include <vector>
#include <array>

class Tile
{
public:
    static std::vector<Tile *> tiles;

    std::array<bool, 256> shouldTick = {false};

    static Tile *empty;
    static Tile *rock;
    static Tile *grass;
    static Tile *dirt;
    static Tile *stoneBrick;
    static Tile *wood;
    static Tile *bush;
    static Tile *bedrock;
    static Tile *water;
    static Tile *calmWater;
    static Tile *lava;
    static Tile *calmLava;

    int id;
    int textureIndex;

    Tile(int id);
    Tile(int id, int tex);
    virtual ~Tile() = default;

    virtual void render(Tesselator &tesselator, Level *level, int x, int y, int z, int size);
    virtual void renderFace(Tesselator &tesselator, int x, int y, int z, int face);
    virtual void renderBackFace(Tesselator &tesselator, int x, int y, int z, int face);
    virtual void renderFaceNoTexture(Player &player, Tesselator &tesselator, int x, int y, int z, int face);
    virtual bool blocksLight();
    virtual bool isSolid();
    virtual bool mayPick();
    virtual void tick(Level *level, int x, int y, int z, std::default_random_engine &random);
    virtual void destroy(Level *level, int x, int y, int z, ParticleEngine &particleEngine);
    AABB *getBlockBoundingBox(int x, int y, int z);
    AABB *getBoundingBox(int x, int y, int z);
    virtual int getLiquidType();
    virtual void neighborChanged(Level *level, int x, int y, int z, int var5);

protected:
    void setTicking(bool shouldTick);
    void setShape(float x0, float y0, float z0, float x1, float y1, float z1);


    virtual int getTexture(int face);
    bool shouldRenderFace(Level *level, int x, int y, int z, int size);

private:
    float x0;
    float y0;
    float z0;
    float x1;
    float y1;
    float z1;


};

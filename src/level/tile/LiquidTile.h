/*

#pragma once

#include "Tile.h"
#include "level/Level.h"
#include "phys/AABB.h"
#include "renderer/Tesselator.h"
#include <random>

class LiquidTile : public Tile {
protected:
    int liquidType;
    int calmTileId;
    int tileId;
    int spreadSpeed = 1;

public:
    LiquidTile(int id, int liquidType);

    void tick(Level *level, int x, int y, int z, std::default_random_engine &random) override;
    bool shouldRenderFace(Level *level, int x, int y, int z, int face) override;
    void renderFace(Tesselator &tesselator, int x, int y, int z, int face) override;
    bool mayPick() override;
    AABB *getBoundingBox(int x, int y, int z) override;
    bool blocksLight() override;
    bool isSolid() override;
    int getLiquidType() override;
    void neighborChanged(Level *level, int x, int y, int z, int tileId) override;

private:
    bool updateWater(Level *level, int x, int y, int z, int depth);
    bool checkWater(Level *level, int x, int y, int z, int depth);
};

 */
/*
#pragma once
#include "level/tile/Tile.h"

class LiquidTile: public Tile {

public:

    void tick(Level *level, int x, int y, int z, std::default_random_engine &random) override;

    void renderFace(Tesselator &tess, Level *level, int x, int y, int z, int face);
    bool mayPick();
    std::optional<AABB> getBoundingBox(int x, int y, int z);
    bool blocksLight();
    bool isSolid();
    int getLiquidType();
    void neighborChanged(Level *level, int x, int y, int z, int var5);


private:
    bool updateWater(Level *level, int x, int y, int z, int var5);
    bool checkWater(Level *level, int x, int y, int z, int var5);
    int spreadSpeed = 1;
protected:
    int liquidType;
    int calmTileId;
    int tileId;

    LiquidTile(int id, int liquidType);
    bool shouldRenderFace(Level *level, int x, int y, int z, int face);
};

*/
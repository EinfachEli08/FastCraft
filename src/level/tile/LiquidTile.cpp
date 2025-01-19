/*
#include "LiquidTile.h"

LiquidTile::LiquidTile(int id, int liquidType) : Tile(id)
{
    this->liquidType = liquidType;
    this->textureIndex = 14;

    if(liquidType == 2) {
        this->textureIndex = 30;
    }

    if(liquidType == 1) {
        this->spreadSpeed = 8;
    }

    if(liquidType == 2) {
        this->spreadSpeed = 2;
    }

    this->tileId = id;
    this->calmTileId = id + 1;
    float var3 = 0.1F;
    //this->setShape(0.0F, 0.0F - var3, 0.0F, 1.0F, 1.0F - var3, 1.0F);
    //this->setTicking(true);
    //TODO
}
*/
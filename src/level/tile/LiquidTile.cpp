/*

#include "LiquidTile.h"

LiquidTile::LiquidTile(int id, int liquidType) : Tile(id), liquidType(liquidType) {
    this->textureIndex = 14;
    if (liquidType == 2) {
        this->textureIndex = 30;
    }

    if (liquidType == 1) {
        this->spreadSpeed = 8;
    }

    if (liquidType == 2) {
        this->spreadSpeed = 2;
    }

    this->tileId = id;
    this->calmTileId = id + 1;
    float var3 = 0.1F;
    this->setShape(0.0F, 0.0F - var3, 0.0F, 1.0F, 1.0F - var3, 1.0F);
    this->setTicking(true);
}

void LiquidTile::tick(Level *level, int x, int y, int z, std::default_random_engine &random) {
    this->updateWater(level, x, y, z, 0);
}

bool LiquidTile::updateWater(Level *level, int x, int y, int z, int depth) {
    bool updated = false;

    bool canUpdate;
    do {
        --y;
        if (level->getTile(x, y, z) != 0) {
            break;
        }

        canUpdate = level->setTile(x, y, z, this->tileId);
        if (canUpdate) {
            updated = true;
        }
    } while (canUpdate && this->liquidType != 2);

    ++y;
    if (this->liquidType == 1 || !updated) {
        updated |= this->checkWater(level, x - 1, y, z, depth);
        updated |= this->checkWater(level, x + 1, y, z, depth);
        updated |= this->checkWater(level, x, y, z - 1, depth);
        updated |= this->checkWater(level, x, y, z + 1, depth);
    }

    if (!updated) {
        level->setTileNoUpdate(x, y, z, this->calmTileId);
    }

    return updated;
}

bool LiquidTile::checkWater(Level *level, int x, int y, int z, int depth) {
    bool updated = false;
    int tile = level->getTile(x, y, z);
    if (tile == 0) {
        bool canUpdate = level->setTile(x, y, z, this->tileId);
        if (canUpdate && depth < this->spreadSpeed) {
            updated = false | this->updateWater(level, x, y, z, depth + 1);
        }
    }

    return updated;
}

bool LiquidTile::shouldRenderFace(Level *level, int x, int y, int z, int face) {
    if (face != 2) {
        return false;
    } else {
        int tile = level->getTile(x, y, z);
        return tile != this->tileId && tile != this->calmTileId ? Tile::shouldRenderFace(level, x, y, z, -1) : false;
    }
}

void LiquidTile::renderFace(Tesselator &tesselator, int x, int y, int z, int face) {
    Tile::renderFace(tesselator, x, y, z, face);
    Tile::renderBackFace(tesselator, x, y, z, face);
}

bool LiquidTile::mayPick() {
    return false;
}

AABB *LiquidTile::getBoundingBox(int x, int y, int z) {
    return nullptr;
}

bool LiquidTile::blocksLight() {
    return true;
}

bool LiquidTile::isSolid() {
    return false;
}

int LiquidTile::getLiquidType() {
    return this->liquidType;
}

void LiquidTile::neighborChanged(Level *level, int x, int y, int z, int tileId) {
    if (this->liquidType == 1 && tileId == Tile::lava->id) {
        level->setTileNoUpdate(x, y, z, Tile::rock->id);
    }

    if (this->liquidType == 2 && tileId == Tile::water->id) {
        level->setTileNoUpdate(x, y, z, Tile::rock->id);
    }
}

*/
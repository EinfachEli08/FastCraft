#include "Tile.h"
#include <random>

#include "level/tile/DirtTile.h"
#include "level/tile/GrassTile.h"
#include "level/tile/Bush.h"
#include "particle/Particle.h"

// Static tile instances
std::vector<Tile *> Tile::tiles(256, nullptr);
Tile *Tile::empty = nullptr;
Tile *Tile::rock = new Tile(1, 1);
Tile *Tile::grass = new GrassTile(2);
Tile *Tile::dirt = new DirtTile(3, 2);
Tile *Tile::stoneBrick = new Tile(4, 16);
Tile *Tile::wood = new Tile(5, 4);
Tile *Tile::bush = new Bush(6);

Tile::Tile(int id) : textureIndex(0)
{
    tiles[id] = this;
    this->id = id;
    this->setShape(0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F);
}

void Tile::setTicking(bool shouldTick) {
    this->shouldTick[this->id] = shouldTick;
}

void Tile::setShape(float x0, float y0, float z0, float x1, float y1, float z1)
{
    this->x0 = x0;
    this->y0 = y0;
    this->z0 = z0;
    this->x1 = x1;
    this->y1 = y1;
    this->z1 = z1;
}

Tile::Tile(int id, int tex) : Tile(id)
{
    this->textureIndex = tex;
}

void Tile::render(Tesselator &tess, Level *level, int x, int y, int z, int size)
{
    if (this->shouldRenderFace(level, y, z - 1, size, x)){
        tess.color(1.0F, 1.0F, 1.0F);
        this->renderFace(tess, y, z, size, 0);
    }

    if (this->shouldRenderFace(level, y, z + 1, size, x)){
        tess.color(1.0F, 1.0F, 1.0F);
        this->renderFace(tess, y, z, size, 1);
    }

    if (this->shouldRenderFace(level, y, z, size - 1, x)){
        tess.color(0.8F, 0.8F, 0.8F);
        this->renderFace(tess, y, z, size, 2);
    }

    if (this->shouldRenderFace(level, y, z, size + 1, x)){
        tess.color(0.8F, 0.8F, 0.8F);
        this->renderFace(tess, y, z, size, 3);
    }

    if (this->shouldRenderFace(level, y - 1, z, size, x)){
        tess.color(0.6F, 0.6F, 0.6F);
        this->renderFace(tess, y, z, size, 4);
    }

    if (this->shouldRenderFace(level, y + 1, z, size, x)){
        tess.color(0.6F, 0.6F, 0.6F);
        this->renderFace(tess, y, z, size, 5);
    }
}

bool Tile::shouldRenderFace(Level *level, int x, int y, int z, int size)
{
    if (x >= 0 && y >= 0 && z >= 0 && x < level->width && y < level->depth && z < level->height){
        bool var6 = true;
        if (size == 2){
            return false;
        } else {
            if (size >= 0)
            {
                var6 = level->isLit(x, y, z) ^ size == 1;
            }

            Tile* var7 = tiles[level->getTile(x, y, z)];
            return !(var7 == nullptr ? false : var7->isSolid()) && var6;
        }
    } else {
        return false;
    }
}

int Tile::getTexture(int face)
{
    return this->textureIndex;
}

void Tile::renderFace(Tesselator &tesselator, int x, int y, int z, int face) {
    int texture = this->getTexture(face);

    float var7 = (float)(texture % 16) / 16.0F;
    float var8 = var7 + 0.999F / 16.0F;
    float var9 = (float)(texture / 16) / 16.0F;
    float var10 = var9 + 0.999F / 16.0F;

    float x1 = x;
    float x2 = x + 1.0f;
    float y1 = y;
    float y2 = y + 1.0f;
    float z1 = z;
    float z2 = z + 1.0f;

    if (face == 0)
    {
        tesselator.vertexUV(x1, y1, z2, var7, var10);
        tesselator.vertexUV(x1, y1, z1, var7, var9);
        tesselator.vertexUV(x2, y1, z1, var8, var9);
        tesselator.vertexUV(x2, y1, z2, var8, var10);
    }

    if (face == 1)
    {
        tesselator.vertexUV(x2, y2, z2, var8, var10);
        tesselator.vertexUV(x2, y2, z1, var8, var9);
        tesselator.vertexUV(x1, y2, z1, var7, var9);
        tesselator.vertexUV(x1, y2, z2, var7, var10);
    }

    if (face == 2)
    {
        tesselator.vertexUV(x1, y2, z1, var8, var9);
        tesselator.vertexUV(x2, y2, z1, var7, var9);
        tesselator.vertexUV(x2, y1, z1, var7, var10);
        tesselator.vertexUV(x1, y1, z1, var8, var10);
    }

    if (face == 3)
    {
        tesselator.vertexUV(x1, y2, z2, var7, var9);
        tesselator.vertexUV(x1, y1, z2, var7, var10);
        tesselator.vertexUV(x2, y1, z2, var8, var10);
        tesselator.vertexUV(x2, y2, z2, var8, var9);
    }

    if (face == 4)
    {
        tesselator.vertexUV(x1, y2, z2, var8, var9);
        tesselator.vertexUV(x1, y2, z1, var7, var9);
        tesselator.vertexUV(x1, y1, z1, var7, var10);
        tesselator.vertexUV(x1, y1, z2, var8, var10);
    }

    if (face == 5)
    {
        tesselator.vertexUV(x2, y1, z2, var7, var10);
        tesselator.vertexUV(x2, y1, z1, var8, var10);
        tesselator.vertexUV(x2, y2, z1, var8, var9);
        tesselator.vertexUV(x2, y2, z2, var7, var9);
    }
}

void Tile::renderBackFace(Tesselator &tesselator, int x, int y, int z, int face) {
    int var6 = this->getTexture(face);

    float var7 = (float)(var6 % 16) / 16.0F;
    float var8 = var7 + 0.999F / 16.0F;
    float var16 = (float)(var6 / 16) / 16.0F;
    float var9 = var16 + 0.999F / 16.0F;

    float var10 = (float)x + this->x0;
    float var14 = (float)x + this->x1;
    float var11 = (float)y + this->y0;
    float var15 = (float)y + this->y1;
    float var12 = (float)z + this->z0;
    float var13 = (float)z + this->z1;

    if(face == 0) {
        tesselator.vertexUV(var14, var11, var13, var8, var9);
        tesselator.vertexUV(var14, var11, var12, var8, var16);
        tesselator.vertexUV(var10, var11, var12, var7, var16);
        tesselator.vertexUV(var10, var11, var13, var7, var9);
    }

    if(face == 1) {
        tesselator.vertexUV(var10, var15, var13, var7, var9);
        tesselator.vertexUV(var10, var15, var12, var7, var16);
        tesselator.vertexUV(var14, var15, var12, var8, var16);
        tesselator.vertexUV(var14, var15, var13, var8, var9);
    }

    if(face == 2) {
        tesselator.vertexUV(var10, var11, var12, var8, var9);
        tesselator.vertexUV(var14, var11, var12, var7, var9);
        tesselator.vertexUV(var14, var15, var12, var7, var16);
        tesselator.vertexUV(var10, var15, var12, var8, var16);
    }

    if(face == 3) {
        tesselator.vertexUV(var14, var15, var13, var8, var16);
        tesselator.vertexUV(var14, var11, var13, var8, var9);
        tesselator.vertexUV(var10, var11, var13, var7, var9);
        tesselator.vertexUV(var10, var15, var13, var7, var16);
    }

    if(face == 4) {
        tesselator.vertexUV(var10, var11, var13, var8, var9);
        tesselator.vertexUV(var10, var11, var12, var7, var9);
        tesselator.vertexUV(var10, var15, var12, var7, var16);
        tesselator.vertexUV(var10, var15, var13, var8, var16);
    }

    if(face == 5) {
        tesselator.vertexUV(var14, var15, var13, var7, var16);
        tesselator.vertexUV(var14, var15, var12, var8, var16);
        tesselator.vertexUV(var14, var11, var12, var8, var9);
        tesselator.vertexUV(var14, var11, var13, var7, var9);
    }
}

void Tile::renderFaceNoTexture(Player &player, Tesselator &tesselator, int x, int y, int z, int face)
{
    float x1 = (float)x;
    float x2 = (float)x + 1.0f;
    float y1 = (float)y;
    float y2 = (float)y + 1.0f;
    float z1 = (float)z;
    float z2 = (float)z + 1.0f;

    if (face == 0 && (float)y > player.y)
    {
        tesselator.vertex(x1, y1, z2);
        tesselator.vertex(x1, y1, z1);
        tesselator.vertex(x2, y1, z1);
        tesselator.vertex(x2, y1, z2);
    }

    if (face == 1 && (float)y < player.y)
    {
        tesselator.vertex(x2, y2, z2);
        tesselator.vertex(x2, y2, z1);
        tesselator.vertex(x1, y2, z1);
        tesselator.vertex(x1, y2, z2);
    }

    if (face == 2 && (float)z > player.z)
    {
        tesselator.vertex(x1, y2, z1);
        tesselator.vertex(x2, y2, z1);
        tesselator.vertex(x2, y1, z1);
        tesselator.vertex(x1, y1, z1);
    }

    if (face == 3 && (float)z < player.z)
    {
        tesselator.vertex(x1, y2, z2);
        tesselator.vertex(x1, y1, z2);
        tesselator.vertex(x2, y1, z2);
        tesselator.vertex(x2, y2, z2);
    }

    if (face == 4 && (float)x > player.x)
    {
        tesselator.vertex(x1, y2, z2);
        tesselator.vertex(x1, y2, z1);
        tesselator.vertex(x1, y1, z1);
        tesselator.vertex(x1, y1, z2);
    }

    if (face == 5 && (float)x < player.x)
    {
        tesselator.vertex(x2, y1, z2);
        tesselator.vertex(x2, y1, z1);
        tesselator.vertex(x2, y2, z1);
        tesselator.vertex(x2, y2, z2);
    }
}

AABB *Tile::getBlockBoundingBox(int x, int y, int z)
{
    return new AABB((float)x, (float)y, (float)z, (float)(x + 1), (float)(y + 1), (float)(z + 1));
}

AABB *Tile::getBoundingBox(int x, int y, int z)
{
    return new AABB((float)x, (float)y, (float)z, (float)(x + 1), (float)(y + 1), (float)(z + 1));
}

bool Tile::blocksLight()
{
    return true;
}

bool Tile::isSolid()
{
    return true;
}

bool Tile::mayPick()
{
    return true;
}

void Tile::tick(Level *level, int x, int y, int z, std::default_random_engine &random) {}

void Tile::destroy(Level *level, int x, int y, int z, ParticleEngine &particleEngine)
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            for (int k = 0; k < 4; ++k)
            {
                float px = (float)x + ((float)i + 0.5f) / (float)4;
                float py = (float)y + ((float)j + 0.5f) / (float)4;
                float pz = (float)z + ((float)k + 0.5f) / (float)4;
                particleEngine.add(new Particle(level, px, py, pz, px - (float)x - 0.5f, py - (float)y - 0.5f, pz - (float)z - 0.5f, this->textureIndex));
            }
        }
    }
}

int Tile::getLiquidType()
{
    return 0;
}

void Tile::neighborChanged(Level *level, int x, int y, int z, int var5) {
}

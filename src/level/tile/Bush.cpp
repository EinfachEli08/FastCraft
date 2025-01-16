#include "Bush.h"

Bush::Bush(int id) : Tile(6)
{
    textureIndex = 15;
}

void Bush::tick(Level *level, int x, int y, int z, std::default_random_engine &random)
{
    int tile = level->getTile(x, y, z);
    if (!level->isLit(x, y, z) || tile != Tile::dirt->id && tile != Tile::grass->id)
    {
        level->setTile(x, y, z, 0);
    }
}

void Bush::render(Tesselator &tess, Level *level, int x, int y, int z, int size)
{
    if (!(level->isLit(x, y, z) ^ size != 1))
    {
        int var15 = this->getTexture(15);
        float var17 = (float)(var15 % 16) / 16.0F;
        float var18 = var17 + 0.999F / 16.0F;
        float var16 = (float)(var15 / 16) / 16.0F;
        float var7 = var16 + 0.999F / 16.0F;
        tess.color((int)255, (int)255, (int)255);

        for (int var8 = 0; var8 < 2; ++var8)
        {
            float var9 = (float)(std::sin((double)var8 * Math::PI / (double)2 + Math::PI * 0.25D) * 0.5D);
            float var10 = (float)(std::cos((double)var8 * Math::PI / (double)2 + Math::PI * 0.25D) * 0.5D);
            float var11 = (float)x + 0.5F - var9;
            var9 += (float)x + 0.5F;
            float var12 = (float)y;
            float var13 = (float)y + 1.0F;
            float var14 = (float)z + 0.5F - var10;
            var10 += (float)z + 0.5F;
            tess.vertexUV(var11, var13, var14, var18, var16);
            tess.vertexUV(var9, var13, var10, var17, var16);
            tess.vertexUV(var9, var12, var10, var17, var7);
            tess.vertexUV(var11, var12, var14, var18, var7);
            tess.vertexUV(var9, var13, var10, var17, var16);
            tess.vertexUV(var11, var13, var14, var18, var16);
            tess.vertexUV(var11, var12, var14, var18, var7);
            tess.vertexUV(var9, var12, var10, var17, var7);
        }
    }
}

std::optional<AABB> Bush::getBoundingBox(int x, int y, int z)
{
    return std::nullopt; // Indicates no valid AABB
}

bool Bush::blocksLight()
{
    return false;
}
bool Bush::isSolid()
{
    return false;
}

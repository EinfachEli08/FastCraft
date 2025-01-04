#include "Bush.h"

Bush::Bush(int id) : Tile(id)
{
    tex = 15;
}

void Bush::tick(Level *level, int x, int y, int z, std::default_random_engine &random)
{
    int tile = level->getTile(x,y,z);
    if(!level->isLit(x,y,z) || tile != Tile::dirt->id && tile != Tile::grass->id){
        level->setTile(x,y,z,0);
    }
}

void Bush::render(Tesselator &tess, Level *level, int x, int y, int z, int size)
{
    if (!(level->isLit(x, y, z) ^ size != 1))
    {
        int var7 = this->getTexture(15);
        float var8 = (float)(var7 % 16) / 16.0F;
        float var9 = var8 + 0.999F / 16.0F;
        float var10 = (float)(var7 / 16) / 16.0F;
        float var11 = var10 + 0.999F / 16.0F;
        double var12 = 2;
        tess.color(1.0F, 1.0F, 1.0F);

        for (int var13 = 0; var13 < var12; ++var13)
        {
            float var14 = (float)(std::sin((double)var13 * Math::PI / (double)var12 + Math::PI * 0.25D) * 0.5D);
            float var15 = (float)(std::cos((double)var13 * Math::PI / (double)var12 + Math::PI * 0.25D) * 0.5D);
            float var16 = (float)x + 0.5F - var14;
            float var17 = (float)x + 0.5F + var14;
            float var18 = (float)y + 0.0F;
            float var19 = (float)y + 1.0F;
            float var20 = (float)z + 0.5F - var15;
            float var21 = (float)z + 0.5F + var15;
            tess.vertexUV(var16, var19, var20, var9, var10);
            tess.vertexUV(var17, var19, var21, var8, var10);
            tess.vertexUV(var17, var18, var21, var8, var11);
            tess.vertexUV(var16, var18, var20, var9, var11);
            tess.vertexUV(var17, var19, var21, var8, var10);
            tess.vertexUV(var16, var19, var20, var9, var10);
            tess.vertexUV(var16, var18, var20, var9, var11);
            tess.vertexUV(var17, var18, var21, var8, var11);
        }
    }
}

std::optional<AABB> Bush::getAABB(int x, int y, int z)
{
    return std::nullopt; // Indicates no valid AABB
}

bool Bush::blocksLight(){
    return false;
}
bool Bush::isSolid(){
    return false;
}


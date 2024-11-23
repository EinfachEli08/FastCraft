#ifndef LEVELRENDERER_HPP
#define LEVELRENDERER_HPP

#include <vector>
#include "Level.h"
#include "Chunk.h"
#include "Tesselator.h"
#include "Player.h"
#include "HitResult.h"
#include "Frustum.h"
#include <GLFW/glfw3.h> // For OpenGL handling

class LevelRenderer : public LevelListener
{
public:
    static constexpr int CHUNK_SIZE = 16;

private:
    Level *level;
    std::vector<Chunk *> chunks;
    int xChunks, yChunks, zChunks;
    Tesselator t;

public:
    LevelRenderer(Level *level);
    ~LevelRenderer();

    
    void render(Player *player, int contextID);

    void renderHit(HitResult hit);

    void pick(Player *player);

    void setDirty(int x0, int y0, int z0, int x1, int y1, int z1);

    void tileChanged(int x, int y, int z) override;

    void lightColumnChanged(int var1, int var2, int var3, int var4);

    void allChanged() override;
};

#endif // LEVELRENDERER_HPP

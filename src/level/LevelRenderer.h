#ifndef LEVELRENDERER_HPP
#define LEVELRENDERER_HPP

#include <vector>
#include "level/Chunk.h"
#include "renderer/Tesselator.h"
#include "Player.h"
#include "HitResult.h"
#include "level/DirtyChunkSorter.h"
#include <GLFW/glfw3.h> // For OpenGL handling

class Level;

class LevelRenderer : public LevelListener
{
public:
    static constexpr int CHUNK_SIZE = 16;
    static constexpr int MAX_REBUILDS_PER_FRAME = 8;

private:
    Level *level;
    std::vector<Chunk *> chunks;
    int xChunks, yChunks, zChunks;
    std::vector<Chunk *> getAllDirtyChunks();
    Textures* textures;

public:
    LevelRenderer(Level *level, Textures* textures);
    ~LevelRenderer();

    void updateDirtyChunks(Player *player);

    void render(Player *player, int contextID);

    void renderHit(Player* player,HitResult hit, int editMode, int paintTexture);

    void setDirty(int x0, int y0, int z0, int x1, int y1, int z1);

    void tileChanged(int x, int y, int z) override;

    void lightColumnChanged(int var1, int var2, int var3, int var4);

    void allChanged() override;
};

#endif // LEVELRENDERER_HPP

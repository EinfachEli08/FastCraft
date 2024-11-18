#ifndef LEVELRENDERER_HPP
#define LEVELRENDERER_HPP

#include <vector>
#include "Level.h"
#include "Chunk.h"
#include "Tesselator.h"
#include "Player.h"
#include "AABB.h"
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

    // Renders the level for the given player and context
    void render(Player *player, int contextID);

    // Handles tile picking logic
    void pick(Player *player);

    // Marks a chunk as dirty, triggering a rebuild
    void setDirty(int x0, int y0, int z0, int x1, int y1, int z1);

    // LevelListener method implementations
    void tileChanged(int x, int y, int z) override;
    void allChanged() override;
};

#endif // LEVELRENDERER_HPP

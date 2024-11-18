#include "LevelRenderer.h"
#include <GLFW/glfw3.h> // For OpenGL handling

LevelRenderer::LevelRenderer(Level *level) : level(level)
{
    level->addListener(this);

    // Calculate how many chunks we have in each dimension
    xChunks = level->width / CHUNK_SIZE;
    yChunks = level->depth / CHUNK_SIZE;
    zChunks = level->height / CHUNK_SIZE;

    // Initialize chunks
    chunks.resize(xChunks * yChunks * zChunks);

    for (int var2 = 0; var2 < xChunks; ++var2)
    {
        for (int var3 = 0; var3 < yChunks; ++var3)
        {
            for (int var4 = 0; var4 < zChunks; ++var4)
            {
                int var5 = var2 * CHUNK_SIZE;
                int var6 = var3 * CHUNK_SIZE;
                int var7 = var4 * CHUNK_SIZE;
                int var8 = (var2 + 1) * CHUNK_SIZE;
                int var9 = (var3 + 1) * CHUNK_SIZE;
                int var10 = (var4 + 1) * CHUNK_SIZE;

                // Clamp the chunk bounds to the level size
                var8 = std::min(var8, level->width);
                var9 = std::min(var9, level->depth);
                var10 = std::min(var10, level->height);

                chunks[(var2 + var3 * xChunks) * zChunks + var4] = new Chunk(level, var5, var6, var7, var8, var9, var10);
            }
        }
    }
}

LevelRenderer::~LevelRenderer()
{
    for (auto &chunk : chunks)
    {
        delete chunk;
    }
}

// Render method
void LevelRenderer::render(Player *player, int contextID)
{
    Chunk::rebuiltThisFrame = 0;

    // Iterate over all chunks and render them
    for (auto &chunk : chunks)
    {
        chunk->render(contextID);
    }
}

// Picking method, used to pick tiles around the player
void LevelRenderer::pick(Player *player)
{
    float selectionRadius = 3.0f;
    AABB pickBox = player->bb.grow(selectionRadius, selectionRadius, selectionRadius);

    int x0 = static_cast<int>(pickBox.x0);
    int x1 = static_cast<int>(pickBox.x1 + 1.0f);
    int y0 = static_cast<int>(pickBox.y0);
    int y1 = static_cast<int>(pickBox.y1 + 1.0f);
    int z0 = static_cast<int>(pickBox.z0);
    int z1 = static_cast<int>(pickBox.z1 + 1.0f);

    glInitNames(); // Initialize name stack for picking

    for (int x = x0; x < x1; ++x)
    {
        glPushName(x);
        for (int y = y0; y < y1; ++y)
        {
            glPushName(y);
            for (int z = z0; z < z1; ++z)
            {
                glPushName(z);
                if (level->isSolidTile(x, y, z))
                {
                    glPushName(0);
                    t.init();

                    // Render the faces of tiles (for example, rocks and grass)
                    Tile::rock.renderFace(t, x, y, z);
                    Tile::grass.renderFace(t, x, y, z);

                    t.flush();
                    glPopName();
                }
                glPopName();
            }
            glPopName();
        }
        glPopName();
    }
}

// Mark chunks as dirty based on the coordinates
void LevelRenderer::setDirty(int x0, int y0, int z0, int x1, int y1, int z1)
{
    x0 /= CHUNK_SIZE;
    x1 /= CHUNK_SIZE;
    y0 /= CHUNK_SIZE;
    y1 /= CHUNK_SIZE;
    z0 /= CHUNK_SIZE;
    z1 /= CHUNK_SIZE;

    x0 = std::max(x0, 0);
    x1 = std::min(x1, xChunks - 1);
    y0 = std::max(y0, 0);
    y1 = std::min(y1, yChunks - 1);
    z0 = std::max(z0, 0);
    z1 = std::min(z1, zChunks - 1);

    // Set the affected chunks as dirty
    for (int x = x0; x <= x1; ++x)
    {
        for (int y = y0; y <= y1; ++y)
        {
            for (int z = z0; z <= z1; ++z)
            {
                chunks[(x + y * xChunks) * zChunks + z]->setDirty();
            }
        }
    }
}

// LevelListener interface method: Called when a tile has changed
void LevelRenderer::tileChanged(int x, int y, int z)
{
    setDirty(x - 1, y - 1, z - 1, x + 1, y + 1, z + 1);
}

// LevelListener interface method: Called when all tiles have changed
void LevelRenderer::allChanged()
{
    setDirty(0, 0, 0, level->width, level->depth, level->height);
}

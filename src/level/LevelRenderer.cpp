#include "LevelRenderer.h"

#include <cmath>
#include <chrono>
#include <algorithm>

LevelRenderer::LevelRenderer(Level *level) : level(level)
{
    level->addListener(this);

    // Calculate how many chunks we have in each dimension
    xChunks = level->width / CHUNK_SIZE;
    yChunks = level->depth / CHUNK_SIZE;
    zChunks = level->height / CHUNK_SIZE;

    // Initialize chunks
    chunks.resize(xChunks * yChunks * zChunks);

    for (int x = 0; x < xChunks; ++x)
    {
        for (int y = 0; y < yChunks; ++y)
        {
            for (int z = 0; z < zChunks; ++z)
            {
                int x0 = x * CHUNK_SIZE;
                int y0 = y * CHUNK_SIZE;
                int z0 = z * CHUNK_SIZE;
                int x1 = (x + 1) * CHUNK_SIZE;
                int y1 = (y + 1) * CHUNK_SIZE;
                int z1 = (z + 1) * CHUNK_SIZE;

                if (x1 > level->width)
                {
                    x1 = level->width;
                }

                if (y1 > level->depth)
                {
                    y1 = level->depth;
                }

                if (z1 > level->height)
                {
                    z1 = level->height;
                }

                chunks[(x + y * xChunks) * zChunks + z] = new Chunk(level, x0, y0, z0, x1, y1, z1);
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

std::vector<Chunk *> LevelRenderer::getAllDirtyChunks()
{
    std::vector<Chunk *> dirtyChunks;

    // Collect all chunks marked as dirty
    for (Chunk *chunk : chunks)
    {
        if (chunk->isDirty())
        {
            dirtyChunks.push_back(chunk);
        }
    }

    return dirtyChunks;
}

void LevelRenderer::render(Player *player, int contextID)
{
    glEnable(GL_TEXTURE_2D);
    GLuint texture;
    try
    {
        texture = Textures::loadTexture("assets/terrain.png", 9728);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    glBindTexture(GL_TEXTURE_2D,texture);

    Frustum frustum = Frustum::getInstance();

    for (int i = 0; i < chunks.size(); ++i)
    {
        if (frustum.cubeInFrustum(chunks[i]->aabb.x0, chunks[i]->aabb.y0, chunks[i]->aabb.z0, chunks[i]->aabb.x1, chunks[i]->aabb.y1, chunks[i]->aabb.z1))
        {
            chunks[i]->render(contextID); // Render the chunk if inside the frustum
        }
    }
    glDisable(GL_TEXTURE_2D);
}

void LevelRenderer::updateDirtyChunks(Player *player)
{
    std::vector<Chunk *> dirtyChunks = this->getAllDirtyChunks();
    if (!dirtyChunks.empty())
    {
      
        DirtyChunkSorter sorter(player, &Frustum::getInstance());
        std::sort(dirtyChunks.begin(), dirtyChunks.end(), sorter);

        for (size_t i = 0; i < std::min<size_t>(8, dirtyChunks.size()); ++i)
        {
            dirtyChunks[i]->rebuild();
        }
    }
}

void LevelRenderer::pick(Player *player)
{
    Tesselator& tess = Tesselator::getInstance();
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
                if (this->level->isSolidTile(x, y, z))
                {
                    glPushName(0);
                    Tile* tile = Tile::tiles[this->level->getTile(x, y, z)];

                    if(tile != nullptr){
                        glPushName(0);
                        for (int face = 0; face < 6; face++)
                        {
                            glPushName(face);
                            tess.init();
                            tile->renderFace(tess, x, y, z, face);
                            tess.flush();
                            glPopName();
                        }
                        glPopName();
                    }
                    glPopName();
                }
                glPopName();
            }
            glPopName();
        }
        glPopName();
    }
}

void LevelRenderer::renderHit(HitResult hit)
{
    Tesselator& tess = Tesselator::getInstance();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glColor4f(1.0f, 1.0f, 1.0f, static_cast<float>(std::sin(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() / 100.0) * 0.2 + 0.4));
    tess.init();
    Tile::rock->renderFaceNoTexture(tess, hit.x, hit.y, hit.z, hit.f);
    tess.flush();
    glDisable(GL_BLEND);
}

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

void LevelRenderer::tileChanged(int x, int y, int z)
{
    setDirty(x - 1, y - 1, z - 1, x + 1, y + 1, z + 1);
}

void LevelRenderer::lightColumnChanged(int var1, int var2, int var3, int var4)
{
    setDirty(var1 - 1, var3 - 1, var2 - 1, var1 + 1, var4 + 1, var2 + 1);
}

void LevelRenderer::allChanged()
{
    setDirty(0, 0, 0, level->width, level->depth, level->height);
}

#include "Chunk.h"

GLuint Chunk::texture = Textures::loadTexture("assets/terrain.png", 9728);
Tesselator Chunk::t;
int Chunk::rebuiltThisFrame = 0;
int Chunk::updates = 0;

Chunk::Chunk(Level *level, int x0, int y0, int z0, int x1, int y1, int z1): level(level), x0(x0), y0(y0), z0(z0), x1(x1), y1(y1), z1(z1), dirty(true)
{
    this->aabb = AABB(static_cast<float>(x0), static_cast<float>(y0), static_cast<float>(z0),
                      static_cast<float>(x1), static_cast<float>(y1), static_cast<float>(z1));
    this->lists = glGenLists(2);
}

void Chunk::rebuild(int var1)
{
    if (rebuiltThisFrame != 2)
    {
        this->dirty = false;
        ++updates;
        ++rebuiltThisFrame;
        glNewList(this->lists + var1, GL_COMPILE);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);
        t.init();

        int tileCount = 0;

        for (int x = this->x0; x < this->x1; ++x)
        {
            for (int y = this->y0; y < this->y1; ++y)
            {
                for (int z = this->z0; z < this->z1; ++z)
                {
                    if (this->level->isTile(x, y, z))
                    {
                        bool isNotGrass = y != this->level->depth * 2 / 3;
                        ++tileCount;
                        if (!isNotGrass)
                        {
                            Tile::rock.render(t,  var1, x, y, z);
                        }
                        else
                        {
                            Tile::grass.render(t,  var1, x, y, z);
                        }
                    }
                }
            }
        }

        t.flush();
        glDisable(GL_TEXTURE_2D);
        glEndList();
    }
}

void Chunk::render(int var1)
{
    if (this->dirty)
    {
        this->rebuild(0);
        this->rebuild(1);
    }

    glCallList(this->lists + var1);
}

void Chunk::setDirty()
{
    this->dirty = true;
}

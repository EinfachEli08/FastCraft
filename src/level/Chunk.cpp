#include "Chunk.h"

#include <glad/glad.h>

GLuint Chunk::texture ;
Tesselator Chunk::t;
int Chunk::rebuiltThisFrame = 0;
int Chunk::updates = 0;

void Chunk::init(GLuint tex)
{
    texture = tex;
}

Chunk::Chunk(Level *level, int x0, int y0, int z0, int x1, int y1, int z1) : level(level), x0(x0), y0(y0), z0(z0), x1(x1), y1(y1), z1(z1), dirty(true)
{
    this->level = level;

    if (!level)
    {
        std::cerr << "Error: level pointer is null" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    this->x0 = x0;
    this->y0 = y0;
    this->z0 = z0;
    this->x1 = x1;
    this->y1 = y1;
    this->z1 = z1;

    this->x = (x0 + x1) / 2.0F;
    this->y = (y0 + y1) / 2.0F;
    this->z = (z0 + z1) / 2.0F;

    
    this->aabb = AABB(static_cast<float>(x0), static_cast<float>(y0),
                      static_cast<float>(z0), static_cast<float>(x1),
                      static_cast<float>(y1), static_cast<float>(z1));

    this->lists = glGenLists(2);
    if (this->lists == 0)
    {
        GLenum error = glGetError();
        std::cerr << "Error: glGenLists failed with error code " << error << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void Chunk::rebuild(int index)
{
    
        this->dirty = false;
        ++updates;
       

        auto start = std::chrono::high_resolution_clock::now();
        auto var2 = std::chrono::duration_cast<std::chrono::nanoseconds>(start.time_since_epoch()).count();

        glNewList(this->lists + index, GL_COMPILE);
        t.init();
        int var4 = 0;
        for (int x = this->x0; x < this->x1; ++x)
        {
            for (int y = this->y0; y < this->y1; ++y)
            {
                for (int z = this->z0; z < this->z1; ++z)
                {
                    int tile = this->level->getTile(x,y,z);
                    if(tile > 0){
                        Tile::tiles[tile]->render(t, this->level, index,x,y,z);
                        var4++;
                    }
                }
            }
        }

        t.flush();
        glEndList();
        auto start1 = std::chrono::high_resolution_clock::now();
        auto var9 = std::chrono::duration_cast<std::chrono::nanoseconds>(start1.time_since_epoch()).count();
        if (var4 > 0)
        {
            this->totalTime += var9-var2;
           ++this->totalUpdates; 
    }
}

void Chunk::rebuild(){
    this->rebuild(0);
    this->rebuild(1);
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
    if(!this->dirty){
        this->dirtiedTime = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
    }
    this->dirty = true;
}

bool Chunk::isDirty()
{
    return this->dirty;
}

float Chunk::distanceToSqr(Player *player)
{
    float dx = player->x - this->x;
    float dy = player->y - this->y;
    float dz = player->z - this->z;
    return dx * dx + dy * dy + dz * dz;
}
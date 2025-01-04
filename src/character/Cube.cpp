#include "utils/OpenGLHeaders.h"
#include "Cube.h"

Cube::Cube(int textureOffsetX, int textureOffsetY)
    : textureOffsetX(textureOffsetX), textureOffsetY(textureOffsetY) {}

void Cube::setTexOffs(int var1, int var2)
{
    this->textureOffsetX = var1;
    this->textureOffsetY = var2;
}

void Cube::addBox(float x, float y, float z, int width, int height, int depth)
{
    vertices.resize(8);
    float x1 = x + width;
    float y1 = y + height;
    float z1 = z + depth;

    vertices[0] = Vertex(x, y, z, 0.0f, 0.0f);
    vertices[1] = Vertex(x1, y, z, 0.0f, 8.0f);
    vertices[2] = Vertex(x1, y1, z, 8.0f, 8.0f);
    vertices[3] = Vertex(x, y1, z, 8.0f, 0.0f);
    vertices[4] = Vertex(x, y, z1, 0.0f, 0.0f);
    vertices[5] = Vertex(x1, y, z1, 0.0f, 8.0f);
    vertices[6] = Vertex(x1, y1, z1, 8.0f, 8.0f);
    vertices[7] = Vertex(x, y1, z1, 8.0f, 0.0f);

    polygons.emplace_back(std::vector<Vertex>{vertices[5], vertices[1], vertices[2], vertices[6]},
                          this->textureOffsetX + depth + width,
                          this->textureOffsetY + depth,
                          this->textureOffsetX + depth + width + depth,
                          this->textureOffsetY + depth + height);
    polygons.emplace_back(std::vector<Vertex>{vertices[0], vertices[4], vertices[7], vertices[3]},
                          this->textureOffsetX + 0,
                          this->textureOffsetY + depth,
                          this->textureOffsetX + depth,
                          this->textureOffsetY + depth + height);
    polygons.emplace_back(std::vector<Vertex>{vertices[5], vertices[4], vertices[0], vertices[1]},
                          this->textureOffsetX + depth,
                          this->textureOffsetY,
                          this->textureOffsetX + depth + width,
                          this->textureOffsetY + depth);
    polygons.emplace_back(std::vector<Vertex>{vertices[2], vertices[3], vertices[7], vertices[6]},
                          this->textureOffsetX + depth + width,
                          this->textureOffsetY,
                          this->textureOffsetX + depth + width + width,
                          this->textureOffsetY + depth);
    polygons.emplace_back(std::vector<Vertex>{vertices[1], vertices[0], vertices[3], vertices[2]},
                          this->textureOffsetX + depth,
                          this->textureOffsetY + depth,
                          this->textureOffsetX + depth + width,
                          this->textureOffsetY + depth + height);
    polygons.emplace_back(std::vector<Vertex>{vertices[4], vertices[5], vertices[6], vertices[7]},
                          this->textureOffsetX + depth + width + depth,
                          this->textureOffsetY + depth,
                          this->textureOffsetX + depth + width + depth + width,
                          this->textureOffsetY + depth + height);
}

void Cube::setPos(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void Cube::render()
{
    if (!this->compiled)
    {
        Cube *var1 = this;
        this->list = glGenLists(1);
        glNewList(this->list, GL_COMPILE);
        glBegin(GL_QUADS);

        for (int var2 = 0; var2 < var1->polygons.size(); ++var2)
        {
            Polygon var3 = var1->polygons[var2];
            glColor3f(1.0F, 1.0F, 1.0F);

            for (int var4 = 3; var4 >= 0; --var4)
            {
                Vertex var5 = var3.vertices[var4];
                glTexCoord2f(var5.u / 63.999F, var5.v / 31.999F);
                glVertex3f(var5.pos.x, var5.pos.y, var5.pos.z);
            }
        }

        glEnd();
        glEndList();
        var1->compiled = true;
    }

    const float DEG_TO_RAD = 57.29578f;
    glPushMatrix();
    glTranslatef(this->x, this->y, this->z);
    glRotatef(this->zRot * DEG_TO_RAD, 0.0f, 0.0f, 1.0f);
    glRotatef(this->yRot * DEG_TO_RAD, 0.0f, 1.0f, 0.0f);
    glRotatef(this->xRot * DEG_TO_RAD, 1.0f, 0.0f, 0.0f);
    glCallList(this->list);
    glPopMatrix();
}
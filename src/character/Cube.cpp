#include "utils/OpenGLHeaders.h"
#include "Cube.h"


Cube::Cube(int xTexOffs, int yTexOffs){
    this->xTexOffs = xTexOffs;
    this->yTexOffs = yTexOffs;
}

void Cube::setTexOffs(int var1, int var2)
{
    this->xTexOffs = var1;
    this->yTexOffs = var2;
}

void Cube::addBox(float x, float y, float z, int width, int height, int depth)
{
    this->vertices.resize(8);
    this->polygons.resize(6);

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

    this->polygons[0] = Polygon(
        {
            vertices[5], 
            vertices[1], 
            vertices[2], 
            vertices[6]
        }, 
        this->xTexOffs + depth + width, 
        this->yTexOffs + depth, 
        this->xTexOffs + depth + width + depth, 
        this->yTexOffs + depth + height
        );
    this->polygons[1] = Polygon(
        {
            vertices[0], 
            vertices[4], 
            vertices[7], 
            vertices[3]
        }, 
        this->xTexOffs + 0, 
        this->yTexOffs + depth, 
        this->xTexOffs + depth, 
        this->yTexOffs + depth + height);
    this->polygons[2] = Polygon(
        {
            vertices[5], 
            vertices[4], 
            vertices[0], 
            vertices[1]
        }, 
        this->xTexOffs + depth, 
        this->yTexOffs, 
        this->xTexOffs + depth + width, 
        this->yTexOffs + depth
        );
    this->polygons[3] = Polygon(
        {
            vertices[2], 
            vertices[3], 
            vertices[7], 
            vertices[6]
        }, 
        this->xTexOffs + depth + width, 
        this->yTexOffs, 
        this->xTexOffs + depth + width + width, 
        this->yTexOffs + depth
        );
    this->polygons[4] = Polygon(
        {
            vertices[1],
            vertices[0], 
            vertices[3], 
            vertices[2]
        }, 
        this->xTexOffs + depth, 
        this->yTexOffs + depth, 
        this->xTexOffs + depth + width, 
        this->yTexOffs + depth + height);
    this->polygons[5] = Polygon(
        {
            vertices[4], 
            vertices[5], 
            vertices[6], 
            vertices[7]
        }, 
        this->xTexOffs + depth + width + depth, 
        this->yTexOffs + depth, 
        this->xTexOffs + depth + width + depth + width, 
        this->yTexOffs + depth + height
        );
}

void Cube::setPos(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

// Render the cube
void Cube::render(){
    if (!this->compiled)
    {
        compile();
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
void Cube::compile()
{
    this->list = glGenLists(1);
    glNewList(this->list, GL_COMPILE);
    glBegin(GL_QUADS);

    for(int i = 0; i < this->polygons.size(); i++)
    {
        this->polygons[i].render();
    }

    glEnd();
    glEndList();
    this->compiled = true;
}

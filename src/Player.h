#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "level/Level.h"
#include <vector>
#include <cstdlib>      // For random numbers
#include <GLFW/glfw3.h> // For keyboard input

class Player
{
private:

public:
    AABB bb;
    Level *level;
    float xo, yo, zo;
    float x, y, z;
    float xd, yd, zd;
    float yRot, xRot;
    bool onGround;

    Player(Level *level);
    void resetPos();
    void setPos(float x, float y, float z);
    void turn(float var1, float var2);
    void tick();
    void move(float var1, float var2, float var3);
    void moveRelative(float var1, float var2, float var3);
};

#endif // PLAYER_HPP

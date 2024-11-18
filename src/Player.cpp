#include "Player.h"
#include "level/Level.h"
#include <GLFW/glfw3.h> // For keyboard input
#include <cmath>        // For sin, cos, sqrt

Player::Player(Level *level) : level(level), xo(0), yo(0), zo(0), x(0), y(0), z(0), xd(0), yd(0), zd(0), yRot(0), xRot(0), onGround(false)                          
{
    resetPos();
}

void Player::resetPos()
{
    // Randomly spawn the player
    float var1 = static_cast<float>(std::rand()) / RAND_MAX * static_cast<float>(level->width);
    float var2 = static_cast<float>(level->depth + 10);
    float var3 = static_cast<float>(std::rand()) / RAND_MAX * static_cast<float>(level->height);
    setPos(var1, var2, var3);
}

void Player::setPos(float var1, float var2, float var3)
{
    x = var1;
    y = var2;
    z = var3;
    float var4 = 0.3f;
    float var5 = 0.9f;
    bb = AABB(var1 - var4, var2 - var5, var3 - var4, var1 + var4, var2 + var5, var3 + var4);
}

void Player::turn(float var1, float var2)
{
    yRot += var1 * 0.15f;
    xRot -= var2 * 0.15f;
    if (xRot < -90.0f)
        xRot = -90.0f;
    if (xRot > 90.0f)
        xRot = 90.0f;
}

void Player::tick()
{
    xo = x;
    yo = y;
    zo = z;
    float var1 = 0.0f;
    float var2 = 0.0f;
    bool down = false;

    // Reset position if R key is pressed
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_R) == GLFW_PRESS)
    {
        resetPos();
    }

    // Movement handling (WASD or arrow keys)
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_UP) == GLFW_PRESS ||
        glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W) == GLFW_PRESS)
    {
        var2 -= 5.0f;
    }

    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_DOWN) == GLFW_PRESS ||
        glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S) == GLFW_PRESS)
    {
        var2 += 5.0f;
    }

    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT) == GLFW_PRESS ||
        glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A) == GLFW_PRESS)
    {
        var1 -= 5.0f;
    }

    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_RIGHT) == GLFW_PRESS ||
        glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D) == GLFW_PRESS)
    {
        var1 += 5.0f;
    }

    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_SPACE) == GLFW_PRESS ||
        glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
    {
        yd = 0.12f;
    }

    // Sprinting (Left Shift)
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        down = true;
    }

    moveRelative(var1, var2, 0.02f);

    if (down)
    {
        yd -= 0.005f;
    }

    move(xd, yd, zd);
    xd *= 0.91f;
    yd *= 0.98f;
    zd *= 0.91f;
    xd *= 0.8f;
    zd *= 0.8f;
}

void Player::move(float var1, float var2, float var3)
{
    float var4 = var1;
    float var5 = var2;
    float var6 = var3;
    std::vector<AABB> collidingCubes = level->getCubes(bb.expand(var1, var2, var3));

    // Handle Y collisions
    for (auto &cube : collidingCubes)
    {
        var2 = cube.clipYCollide(bb, var2);
    }
    bb.move(0.0f, var2, 0.0f);

    // Handle X collisions
    for (auto &cube : collidingCubes)
    {
        var1 = cube.clipXCollide(bb, var1);
    }
    bb.move(var1, 0.0f, 0.0f);

    // Handle Z collisions
    for (auto &cube : collidingCubes)
    {
        var3 = cube.clipZCollide(bb, var3);
    }
    bb.move(0.0f, 0.0f, var3);

    onGround = var5 != var2 && var5 < 0.0f;

    if (var4 != var1)
        xd = 0.0f;
    if (var5 != var2)
        yd = 0.0f;
    if (var6 != var3)
        zd = 0.0f;

    // Update player position based on AABB
    x = (bb.x0 + bb.x1) / 2.0f;
    y = bb.y0 + 1.62f;
    z = (bb.z0 + bb.z1) / 2.0f;
}

void Player::moveRelative(float var1, float var2, float var3)
{

    float var4 = var1 * var1 + var2 * var2;
    if (var4 >= 0.01f)
    {
        var4 = var3 / std::sqrt(var4);
        var1 *= var4;
        var2 *= var4;
        float var5 = std::sin(yRot * 3.14159265359 / 180.0f);
        float var6 = std::cos(yRot * 3.14159265359 / 180.0f);
        xd += var1 * var6 - var2 * var5;
        zd += var2 * var6 + var1 * var5;
    }
}

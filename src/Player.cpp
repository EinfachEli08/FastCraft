#include "Player.h"
#include <GLFW/glfw3.h> // For keyboard input
#include <cmath>        // For sin, cos, sqrt

// Constructor
Player::Player(Level *level) : Entity(level)
{
    this->heightOffset = 1.62F;
}


// Handle input and physics
void Player::tick()
{
    this->xo = this->x;
    this->yo = this->y;
    this->zo = this->z;
    float var1 = 0.0f;
    float var2 = 0.0f;

    // Keyboard input
    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_R) == GLFW_PRESS)
        this->resetPos();

    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W) == GLFW_PRESS ||
        glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_UP) == GLFW_PRESS)
        --var2;

    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S) == GLFW_PRESS ||
        glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_DOWN) == GLFW_PRESS)
        ++var2;

    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A) == GLFW_PRESS ||
        glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT) == GLFW_PRESS)
        --var1;

    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D) == GLFW_PRESS ||
        glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_RIGHT) == GLFW_PRESS)
        ++var1;

    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_SPACE) == GLFW_PRESS && onGround)
        this->yd = 0.5f;
    //}

    // Movement and physics updates
    this->moveRelative(var1, var2, this->onGround ? 0.1f : 0.02f);
    this->yd = this->yd - 0.08D;
    this->move(this->xd, this->yd, this->zd);
    this->xd *= 0.91f;
    this->yd *= 0.98f;
    this->zd *= 0.91f;

    if (this->onGround)
    {
        this->xd *= 0.7f;
        this->zd *= 0.7f;
    }
}
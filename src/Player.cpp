#include "Player.h"
#include <GLFW/glfw3.h>
#include <cmath>

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

    bool var3 = false; // Placeholder for water interaction
    bool var4 = false; // Placeholder for lava interaction



    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_R) == GLFW_PRESS){
        this->resetPos();
    }

    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_UP) == GLFW_PRESS){
        var2 = 0.0F - 1.0F;
    }

    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_DOWN) == GLFW_PRESS){
        ++var2;
    }


    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT) == GLFW_PRESS){
        var1 = 0.0F - 1.0F;
    }


    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_RIGHT) == GLFW_PRESS){
        ++var1;
    }


    if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_SPACE) == GLFW_PRESS && onGround){
        if(var3) {
            this->yd += 0.06F;
        } else if(var4) {
            this->yd += 0.04F;
        } else if(this->onGround) {
            this->yd = 0.5F;
        }
    }

    if(var3) {
        this->moveRelative(var1, var2, 0.02F);
        this->move(this->xd, this->yd, this->zd);
        this->xd *= 0.7F;
        this->yd *= 0.7F;
        this->zd *= 0.7F;
        this->yd = (float)((double)this->yd - 0.02D);
    } else if(var4) {
        this->moveRelative(var1, var2, 0.02F);
        this->move(this->xd, this->yd, this->zd);
        this->xd *= 0.5F;
        this->yd *= 0.5F;
        this->zd *= 0.5F;
        this->yd = (float)((double)this->yd - 0.02D);
    } else {
        this->moveRelative(var1, var2, this->onGround ? 0.1F : 0.02F);
        this->move(this->xd, this->yd, this->zd);
        this->xd *= 0.91F;
        this->yd *= 0.98F;
        this->zd *= 0.91F;
        this->yd = (float)((double)this->yd - 0.08D);
        if(this->onGround) {
            this->xd *= 0.6F;
            this->zd *= 0.6F;
        }
    }
}
#include "Mouse.h"

// Static instance to allow callback to access class
static Mouse *instance = nullptr;

// Constructor
Mouse::Mouse() : currentX(0.0), currentY(0.0), prevX(0.0), prevY(0.0), deltaX(0.0), deltaY(0.0)
{
    instance = this;
}

// Update method
void Mouse::update(double xpos, double ypos)
{
    // Calculate deltas
    deltaX = xpos - prevX;
    deltaY = ypos - prevY;

    // Update positions
    prevX = currentX;
    prevY = currentY;

    currentX = xpos;
    currentY = ypos;
}

// Get current X position
double Mouse::getX() const
{
    return currentX;
}

// Get current Y position
double Mouse::getY() const
{
    return currentY;
}

// Get delta X
double Mouse::getDX() const
{
    return deltaX;
}

// Get delta Y
double Mouse::getDY() const
{
    return deltaY;
}

// Static callback method
void Mouse::cursorPositionCallback(GLFWwindow *window, double xpos, double ypos)
{
    if (instance)
    {
        instance->update(xpos, -ypos);
    }
}

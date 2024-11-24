#include "Mouse.h"
#include <GLFW/glfw3.h> // Include GLFW for mouse state functions

// Static instance to allow access to the class
static Mouse *instance = nullptr;

// Constructor
Mouse::Mouse() : currentX(0.0), currentY(0.0), prevX(0.0), prevY(0.0), deltaX(0.0), deltaY(0.0), eventButton(-1), eventButtonState(GLFW_RELEASE)
{
    instance = this;
}

// Update method for position and button states
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

    // Poll mouse button states for event tracking
    GLFWwindow *currentContext = glfwGetCurrentContext();
    if (currentContext)
    {
        for (int button = 0; button <= GLFW_MOUSE_BUTTON_LAST; ++button)
        {
            int state = glfwGetMouseButton(currentContext, button);
            if (state == GLFW_PRESS && buttonStates[button] == GLFW_RELEASE)
            {
                // Button was just pressed
                eventButton = button;
                eventButtonState = GLFW_PRESS;
            }
            else if (state == GLFW_RELEASE && buttonStates[button] == GLFW_PRESS)
            {
                // Button was just released
                eventButton = button;
                eventButtonState = GLFW_RELEASE;
            }

            // Update internal button state
            buttonStates[button] = state;
        }
    }
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

// Check if a specific button is pressed
bool Mouse::isButtonPressed(int button) const
{
    GLFWwindow *currentContext = glfwGetCurrentContext();
    if (!currentContext)
    {
        return false; // Ensure there's a valid context
    }
    return glfwGetMouseButton(currentContext, button) == GLFW_PRESS;
}

// Get the last event button
int Mouse::getEventButton() const
{
    return eventButton;
}

// Get the state of the last event button
int Mouse::getEventButtonState() const
{
    return eventButtonState;
}

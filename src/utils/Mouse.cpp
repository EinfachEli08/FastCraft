#include "Mouse.h"
#include <iostream>
#include <GLFW/glfw3.h> // Include GLFW for mouse state functions

// Static instance to allow access to the class
static Mouse *instance = nullptr;

// Constructor
Mouse::Mouse() : currentX(0.0), currentY(0.0), prevX(0.0), prevY(0.0), deltaX(0.0), deltaY(0.0),
                 eventButton(-1), eventButtonState(GLFW_RELEASE)
{
    instance = this;

    // Initialize all button states
    for (int i = 0; i <= GLFW_MOUSE_BUTTON_LAST; ++i)
    {
        buttonStates[i] = GLFW_RELEASE; // Initial state
        buttonClick[i] = false;         // Not clicked
        buttonRelease[i] = false;       // Not released
    }
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

            // Handle click (triggered once)
            if (state == GLFW_PRESS && buttonStates[button] == GLFW_RELEASE)
            {
                buttonClick[button] = true; // Mark as clicked
            }
            else
            {
                buttonClick[button] = false; // Reset click state
            }

            // Handle release (triggered once)
            if (state == GLFW_RELEASE && buttonStates[button] == GLFW_PRESS)
            {
                buttonRelease[button] = true; // Mark as released
            }
            else
            {
                buttonRelease[button] = false; // Reset release state
            }

            // Update internal button state
            buttonStates[button] = state;
        }
    }
}

// Poll method for mouse events
void Mouse::poll()
{
    // Poll mouse position
    GLFWwindow *currentContext = glfwGetCurrentContext();
    if (currentContext)
    {
        double xpos, ypos;
        glfwGetCursorPos(currentContext, &xpos, &ypos);
        update(xpos, ypos);
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

// Check if a specific button is currently held down
bool Mouse::isButtonHeld(int button) const
{
    std::cout << "Button held: " << button << std::endl;

    if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST)
        return false;
    return buttonStates[button] == GLFW_PRESS;
}

// Check if a specific button was clicked
bool Mouse::isButtonClicked(int button) const
{
    std::cout << "Button clicked: " << button << std::endl;

    if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST)
        return false;
    return buttonClick[button];
}

// Check if a specific button was released
bool Mouse::isButtonReleased(int button) const
{
    std::cout << "Button released: " << button << std::endl;
    if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST)
        return false;
    
    return buttonRelease[button];
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

// Set cursor position
void Mouse::setCursorPosition(double xpos, double ypos)
{
    GLFWwindow *currentContext = glfwGetCurrentContext();
    if (currentContext)
    {
        glfwSetCursorPos(currentContext, xpos, ypos);
        update(xpos, ypos);
    }
}

bool Mouse::next()
{
    for (int button = 0; button <= GLFW_MOUSE_BUTTON_LAST; ++button)
    {
        if (buttonClick[button] || buttonRelease[button])
        {
            eventButton = button;
            eventButtonState = buttonStates[button];
            resetButtonEvents();
            return true;
        }
    }
    return false;
}

// Helper method to reset button events
void Mouse::resetButtonEvents()
{
    for (int i = 0; i <= GLFW_MOUSE_BUTTON_LAST; ++i)
    {
        buttonClick[i] = false;
        buttonRelease[i] = false;
    }
}

#include "Keyboard.h"

static Keyboard *instance = nullptr;

Keyboard::Keyboard() : eventKey(-1), eventKeyState(GLFW_RELEASE)
{
    instance = this;

    // Initialize all key states
    for (int i = 0; i <= GLFW_KEY_LAST; ++i)
    {
        keyStates[i] = GLFW_RELEASE; // Initial state
        keyPress[i] = false;         // Not pressed
        keyRelease[i] = false;       // Not released
    }
}

// Update method for key states
void Keyboard::update()
{
    GLFWwindow *currentContext = glfwGetCurrentContext();
    if (currentContext)
    {
        for (int key = 0; key <= GLFW_KEY_LAST; ++key)
        {
            int state = glfwGetKey(currentContext, key);

            // Handle press (triggered once)
            if (state == GLFW_PRESS && keyStates[key] == GLFW_RELEASE)
            {
                keyPress[key] = true; // Mark as pressed
            }
            else
            {
                keyPress[key] = false; // Reset press state
            }

            // Handle release (triggered once)
            if (state == GLFW_RELEASE && keyStates[key] == GLFW_PRESS)
            {
                keyRelease[key] = true; // Mark as released
            }
            else
            {
                keyRelease[key] = false; // Reset release state
            }

            // Update internal key state
            keyStates[key] = state;
        }
    }
}

// Get the last event key
int Keyboard::getEventKey() const
{
    return eventKey;
}

// Get the state of the last event key
int Keyboard::getEventKeyState() const
{
    return eventKeyState;
}

// Check if a specific key is currently held down
bool Keyboard::isKeyHeld(int key) const
{
    if (key < 0 || key > GLFW_KEY_LAST)
        return false;
    return keyStates[key] == GLFW_PRESS;
}

// Check if a specific key was pressed
bool Keyboard::isKeyPressed(int key) const
{
    if (key < 0 || key > GLFW_KEY_LAST)
        return false;
    return keyPress[key];
}

// Check if a specific key was released
bool Keyboard::isKeyReleased(int key) const
{
    if (key < 0 || key > GLFW_KEY_LAST)
        return false;
    return keyRelease[key];
}

// Static method to get the instance
Keyboard *Keyboard::getInstance()
{
    if (!instance)
    {
        instance = new Keyboard();
    }
    return instance;
}

// Move to the next key event
bool Keyboard::next()
{
    // Iterate through all keys to find the next event
    for (int key = 0; key <= GLFW_KEY_LAST; ++key)
    {
        if (keyPress[key] || keyRelease[key])
        {
            eventKey = key;
            eventKeyState = keyStates[key];
            resetKeyEvents();
            return true;
        }
    }
    return false;
}

// Helper method to reset key events
void Keyboard::resetKeyEvents()
{
    for (int i = 0; i <= GLFW_KEY_LAST; ++i)
    {
        keyPress[i] = false;
        keyRelease[i] = false;
    }
}
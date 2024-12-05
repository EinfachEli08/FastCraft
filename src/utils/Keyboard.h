#pragma once

#include <array>
#include <GLFW/glfw3.h>

class Keyboard
{
public:
    Keyboard();

    // Update key states
    void update();

    // Get last event key and its state
    int getEventKey() const;
    int getEventKeyState() const;

    // Key state checks
    bool isKeyHeld(int key) const;
    bool isKeyPressed(int key) const;
    bool isKeyReleased(int key) const;

    // Static instance access
    static Keyboard *getInstance();

private:
    // Key states and event tracking
    std::array<int, GLFW_KEY_LAST + 1> keyStates;
    
    std::array<bool, GLFW_KEY_LAST + 1> keyPress;
    std::array<bool, GLFW_KEY_LAST + 1> keyRelease;

    int eventKey;      // Last event key
    int eventKeyState; // Last event key state
};


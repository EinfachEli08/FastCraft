#ifndef MOUSE_H
#define MOUSE_H

#include <GLFW/glfw3.h> // Include GLFW for mouse input

class Mouse
{
public:
    // Constructor
    Mouse();

    // Update method to track position and button states
    void update(double xpos, double ypos);

    // Get current mouse position
    double getX() const;
    double getY() const;

    // Get mouse position deltas
    double getDX() const;
    double getDY() const;

    // Check if a specific mouse button is currently pressed
    bool isButtonPressed(int button) const;

    // Get the last button that triggered an event
    int getEventButton() const;

    // Get the state of the last event button (GLFW_PRESS or GLFW_RELEASE)
    int getEventButtonState() const;

private:
    // Mouse position
    double currentX;
    double currentY;
    double prevX;
    double prevY;

    // Position deltas
    double deltaX;
    double deltaY;

    // State tracking
    int eventButton;                              // Last button to trigger an event
    int eventButtonState;                         // State of the last event button
    int buttonStates[GLFW_MOUSE_BUTTON_LAST + 1]; // Current states of all mouse buttons
};

#endif // MOUSE_H

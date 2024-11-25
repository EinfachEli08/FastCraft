#ifndef MOUSE_H
#define MOUSE_H

#include <array>        // For button state arrays
#include <GLFW/glfw3.h> // GLFW for mouse constants

class Mouse
{
public:
    Mouse(); // Constructor

    // Update method to update mouse state
    void update(double xpos, double ypos);

    // Accessors for mouse position
    double getX() const;
    double getY() const;
    double getDX() const;
    double getDY() const;

    // Methods to check mouse button states
    bool isButtonHeld(int button) const;     // Check if button is held
    bool isButtonClicked(int button) const;  // Check if button is clicked
    bool isButtonReleased(int button) const; // Check if button is released

    // Event button information
    int getEventButton() const;
    int getEventButtonState() const;

private:
    // Mouse position
    double currentX, currentY; // Current position
    double prevX, prevY;       // Previous position
    double deltaX, deltaY;     // Position deltas

    // Button state tracking
    std::array<int, GLFW_MOUSE_BUTTON_LAST + 1> buttonStates;   // Current states of buttons
    std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> buttonClick;   // Clicked state
    std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> buttonRelease; // Released state

    // Last event button
    int eventButton;
    int eventButtonState;
};

#endif // MOUSE_H

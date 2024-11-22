#ifndef MOUSE_H
#define MOUSE_H

#include <GLFW/glfw3.h>

class Mouse
{
private:
    // Current and previous positions
    double currentX, currentY;
    double prevX, prevY;

    // Delta values
    double deltaX, deltaY;

public:
    // Constructor
    Mouse();

    // Update method (to be called in cursor position callback)
    void update(double xpos, double ypos);

    // Getters for position
    double getX() const;
    double getY() const;

    // Getters for delta
    double getDX() const;
    double getDY() const;

    // Callback registration helper
    static void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos);
};

#endif // MOUSE_H

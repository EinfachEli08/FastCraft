#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <optional>

class Controller
{
public:
    Controller(int controllerID);

    // Update the controller state
    void update();

    // Check if the controller is connected
    bool isConnected() const;

    // Get the state of a button (pressed or not)
    bool isButtonPressed(int button) const;

    // Get the position of an axis (e.g., joystick)
    float getAxisPosition(int axis) const;

    // Get the delta (change) of an axis position
    float getDX(int axis) const;
    float getDY(int axis) const;

    // Get the controller name
    std::string getControllerName() const;

    // Get the currently pressed button, if any
    std::optional<int> getPressedButton() const;

private:
    int m_controllerID;
    bool m_isConnected;
    std::vector<unsigned char> m_buttons;
    std::vector<float> m_axes;
    std::vector<float> m_prevAxes; // Store previous axis states
    std::string m_name;

    void checkConnection();
    void updateState();
};

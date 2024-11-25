#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <vector>
#include <string>
#include <optional>

class Controller
{
public:
    Controller(int controllerID);

    // Update the controller's state
    void update();

    // Check connection status
    bool isConnected() const;

    // Button methods
    bool isButtonPressed(int button) const;
    std::optional<int> getPressedButton() const;

    // Axis methods
    float getAxisPosition(int axis) const;
    float getDX(int axis) const;
    float getDY(int axis) const;

    // Axis as button methods
    bool isAxisButtonPressed(int axis) const;
    bool isAxisButtonReleased(int axis) const;
    bool isAxisButtonHeld(int axis) const;

    // Controller information
    std::string getControllerName() const;

    // Deadzone configuration
    void setAxisDeadzone(float deadzone);

private:
    void checkConnection();                 // Check if the controller is connected
    void updateState();                     // Update button and axis states
    float applyDeadzone(float value) const; // Apply deadzone to an axis value

    int m_controllerID; // Controller ID
    bool m_isConnected; // Connection status
    std::string m_name; // Controller name

    std::vector<unsigned char> m_buttons; // Button states
    std::vector<float> m_axes;            // Current axis states
    std::vector<float> m_prevAxes;        // Previous axis states

    std::vector<bool> m_axisButtonPressed;  // Axis button pressed state
    std::vector<bool> m_axisButtonReleased; // Axis button released state

    const float m_axisButtonThreshold = 0.5f; // Threshold for axis button activation
    float m_deadzone = 0.1f;                  // Default deadzone value for axes
};

#endif // CONTROLLER_H

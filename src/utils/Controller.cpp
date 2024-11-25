#include "Controller.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath> // For std::abs

Controller::Controller(int controllerID)
    : m_controllerID(controllerID), m_isConnected(false)
{
    checkConnection();
}

void Controller::update()
{
    checkConnection();
    if (m_isConnected)
    {
        updateState();
    }
}

bool Controller::isConnected() const
{
    return m_isConnected;
}

bool Controller::isButtonPressed(int button) const
{
    if (!m_isConnected || button < 0 || button >= m_buttons.size())
    {
        return false;
    }
    return m_buttons[button] == GLFW_PRESS;
}

float Controller::getAxisPosition(int axis) const
{
    if (!m_isConnected || axis < 0 || axis >= m_axes.size())
    {
        return 0.0f;
    }
    return applyDeadzone(m_axes[axis]);
}

float Controller::getDX(int axis) const
{
    if (!m_isConnected || axis < 0 || axis >= m_axes.size())
    {
        return 0.0f;
    }
    return applyDeadzone(m_axes[axis]) - applyDeadzone(m_prevAxes[axis]);
}

float Controller::getDY(int axis) const
{
    if (!m_isConnected || axis < 0 || axis >= m_axes.size())
    {
        return 0.0f;
    }
    return applyDeadzone(m_axes[axis]) - applyDeadzone(m_prevAxes[axis]);
}

std::string Controller::getControllerName() const
{
    return m_name;
}

std::optional<int> Controller::getPressedButton() const
{
    if (!m_isConnected)
    {
        return std::nullopt;
    }
    for (int i = 0; i < m_buttons.size(); ++i)
    {
        if (m_buttons[i] == GLFW_PRESS)
        {
            return i;
        }
    }
    return std::nullopt;
}

bool Controller::isAxisButtonPressed(int axis) const
{
    if (!m_isConnected || axis < 0 || axis >= m_axisButtonPressed.size())
    {
        return false;
    }

    return m_axisButtonPressed[axis];
}

bool Controller::isAxisButtonReleased(int axis) const
{
    if (!m_isConnected || axis < 0 || axis >= m_axisButtonReleased.size())
    {
        return false;
    }
    return m_axisButtonReleased[axis];
}

bool Controller::isAxisButtonHeld(int axis) const
{
    if (!m_isConnected || axis < 0 || axis >= m_axes.size())
    {
        return false;
    }
    return std::abs(applyDeadzone(m_axes[axis])) >= m_axisButtonThreshold;
}

void Controller::checkConnection()
{
    if (glfwJoystickPresent(m_controllerID))
    {
        m_isConnected = true;
        m_name = glfwGetJoystickName(m_controllerID);
    }
    else
    {
        m_isConnected = false;
        m_name.clear();
        m_buttons.clear();
        m_axes.clear();
        m_prevAxes.clear();
        m_axisButtonPressed.clear();
        m_axisButtonReleased.clear();
    }
}

void Controller::updateState()
{
    if (!m_isConnected)
        return;

    int buttonCount = 0;
    int axisCount = 0;

    const unsigned char *buttons = glfwGetJoystickButtons(m_controllerID, &buttonCount);
    const float *axes = glfwGetJoystickAxes(m_controllerID, &axisCount);

    m_buttons.assign(buttons, buttons + buttonCount);

    if (m_axes.size() != axisCount)
    {
        m_prevAxes.assign(axisCount, 0.0f);
        m_axes.assign(axisCount, 0.0f);
        m_axisButtonPressed.assign(axisCount, false);
        m_axisButtonReleased.assign(axisCount, false);
    }

    m_prevAxes = m_axes; // Store previous state
    m_axes.assign(axes, axes + axisCount);

    // Update axis button states
    for (int i = 0; i < axisCount; ++i)
    {
        float current = applyDeadzone(m_axes[i]);
        float previous = applyDeadzone(m_prevAxes[i]);

        // Determine if the axis is currently pressed based on the threshold
        bool currentlyPressed = std::abs(current) >= m_axisButtonThreshold;
        bool previouslyPressed = std::abs(previous) >= m_axisButtonThreshold;

        // Prevent double triggers by only setting flags when a transition is detected
        m_axisButtonPressed[i] = currentlyPressed && !previouslyPressed;  // Transition from not pressed to pressed
        m_axisButtonReleased[i] = previouslyPressed && !currentlyPressed; // Transition from pressed to not pressed

        // If there's no transition, clear the flags
        if (!m_axisButtonPressed[i])
            m_axisButtonPressed[i] = false;

        if (!m_axisButtonReleased[i])
            m_axisButtonReleased[i] = false;
    }
}

float Controller::applyDeadzone(float value) const
{
    return (std::abs(value) < m_deadzone) ? 0.0f : value;
}

void Controller::setAxisDeadzone(float deadzone)
{
    m_deadzone = std::max(0.0f, deadzone); // Ensure deadzone is non-negative
}

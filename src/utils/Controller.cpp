#include "Controller.h"
#include <iostream>

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
    return m_axes[axis];
}

float Controller::getDX(int axis) const
{
    if (!m_isConnected || axis < 0 || axis >= m_axes.size())
    {
        return 0.0f;
    }
    return m_axes[axis] - m_prevAxes[axis];
}

float Controller::getDY(int axis) const
{
    if (!m_isConnected || axis < 0 || axis >= m_axes.size())
    {
        return 0.0f;
    }
    return m_axes[axis] - m_prevAxes[axis];
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
    }

    m_prevAxes = m_axes; // Store previous state
    m_axes.assign(axes, axes + axisCount);
}

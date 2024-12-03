#pragma once

#include <vector>
#include <string>
#include <optional>

class Controller
{
public:
    Controller(int controllerID);

    void update();

    bool isConnected() const;

    bool isButtonPressed(int button) const;
    std::optional<int> getPressedButton() const;

    float getAxisPosition(int axis) const;
    float getDX(int axis) const;
    float getDY(int axis) const;

    bool isAxisButtonPressed(int axis) const;
    bool isAxisButtonReleased(int axis) const;
    bool isAxisButtonHeld(int axis) const;

    std::string getControllerName() const;

    void setAxisDeadzone(float deadzone);

private:
    void checkConnection();                 
    void updateState();          
    float applyDeadzone(float value) const; 

    int m_controllerID; 
    bool m_isConnected; 
    std::string m_name; 

    std::vector<unsigned char> m_buttons; 
    std::vector<float> m_axes;           
    std::vector<float> m_prevAxes;       

    std::vector<bool> m_axisButtonPressed;  
    std::vector<bool> m_axisButtonReleased; 

    const float m_axisButtonThreshold = 0.5f; 
    float m_deadzone = 0.1f;                 
};



#pragma once

#include <array>       
#include <GLFW/glfw3.h> 

class Mouse
{
public:
    Mouse(); 

    void update(double xpos, double ypos);

    double getX() const;
    double getY() const;
    double getDX() const;
    double getDY() const;

    bool isButtonHeld(int button) const;    
    bool isButtonClicked(int button) const; 
    bool isButtonReleased(int button) const; 

    int getEventButton() const;
    int getEventButtonState() const;

private:
    double currentX, currentY;
    double prevX, prevY;      
    double deltaX, deltaY;     

    std::array<int, GLFW_MOUSE_BUTTON_LAST + 1> buttonStates;   
    std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> buttonClick;  
    std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> buttonRelease;

    int eventButton;
    int eventButtonState;
};


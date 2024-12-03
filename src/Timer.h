#pragma once

class Timer
{
private:
    static const long NS_PER_SECOND = 1000000000L;
    static const long MAX_NS_PER_UPDATE = 1000000000L;
    static const int MAX_TICKS_PER_UPDATE = 100;

    float ticksPerSecond;
    long lastTime;
   
   
    float timeScale;
    float fps;
    float passedTime;

public:
    int ticks;
    Timer(float ticksPerSecond);
    void advanceTime();
    int getTicks() const;
    float getFPS() const;
    float getPassedTime() const;
    float getTimeScale() const;
    void setTimeScale(float scale);
    float a;
};


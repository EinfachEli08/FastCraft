#include "Timer.h"
#include <chrono>

Timer::Timer(float ticksPerSecond)
    : ticksPerSecond(ticksPerSecond), lastTime(std::chrono::high_resolution_clock::now().time_since_epoch().count()),
      ticks(0), a(0.0f), timeScale(1.0f), fps(0.0f), passedTime(0.0f) {}

void Timer::advanceTime()
{
    long var1 = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    long var3 = var1 - lastTime;
    lastTime = var1;

    if (var3 < 0L)
    {
        var3 = 0L;
    }

    if (var3 > 1000000000L)
    {
        var3 = 1000000000L;
    }

    fps = static_cast<float>(1000000000L) / static_cast<float>(var3);
    passedTime += static_cast<float>(var3) * timeScale * ticksPerSecond / 1.0E9f;
    ticks = static_cast<int>(passedTime);

    if (ticks > 100)
    {
        ticks = 100;
    }

    passedTime -= static_cast<float>(ticks);
    a = passedTime;
}

int Timer::getTicks() const
{
    return ticks;
}

float Timer::getFPS() const
{
    return fps;
}

float Timer::getPassedTime() const
{
    return passedTime;
}

float Timer::getTimeScale() const
{
    return timeScale;
}

void Timer::setTimeScale(float scale)
{
    timeScale = scale;
}

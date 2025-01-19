#include "Timer.h"
#include <chrono>

Timer::Timer(float ticksPerSecond)
    : ticksPerSecond(ticksPerSecond), lastTime(std::chrono::high_resolution_clock::now().time_since_epoch().count()),
      ticks(0), a(0.0f), timeScale(1.0f), fps(0.0f), passedTime(0.0f) {}

void Timer::advanceTime()
{
    long var1 = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    long var3 = var1 - lastTime;
    this->lastTime = var1;

    if (var3 < 0L)
    {
        var3 = 0L;
    }

    if (var3 > 1000000000L)
    {
        var3 = 1000000000L;
    }

    this->fps = (float)1000000000L / var3;
    this->passedTime += (float)var3 *  this->timeScale *  this->ticksPerSecond / 1.0E9f;
    this->ticks = (int)this->passedTime;

    if (this->ticks > 100)
    {
        this->ticks = 100;
    }

    this->passedTime -= (float)this->ticks;
    this->a = this->passedTime;
}


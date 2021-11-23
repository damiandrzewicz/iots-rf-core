#pragma once 

#include <LowPower.h>
#include <Arduino.h>

class LowPowerWrapper
{
public:
    void DeepSleep(uint32_t sleepTime);
    void DeepSleepForever();
};

extern LowPowerWrapper LowPowerWrp;
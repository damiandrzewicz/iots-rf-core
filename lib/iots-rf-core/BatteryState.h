#pragma once
#include "Arduino.h"

class BatteryState
{
public:
    BatteryState();

    void setRange(uint16_t mV_0prc, uint16_t mV_100prc);
    virtual void read();
    uint8_t getCurrent_mV();
    uint8_t computePercent();
    void setOmmitCycles(uint8_t ommitCycles);
    uint8_t getOmmitCycles();

private:
    uint16_t mV_act_ = 0;
    uint16_t mV_0prc_ = 1850;
    uint16_t mV_100prc_ = 2950;
    uint8_t ommitCycles_ = 30;
};
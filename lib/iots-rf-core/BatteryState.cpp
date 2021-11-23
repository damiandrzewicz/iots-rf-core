#include "BatteryState.h"

BatteryState::BatteryState()
{

}

void BatteryState::setRange(uint16_t mV_0prc, uint16_t mV_100prc)
{
    mV_0prc_ = mV_0prc;
    mV_100prc_ = mV_100prc;
}

void BatteryState::read()
{
    // Read 1.1V reference against AVcc
    // set the reference to Vcc and the measurement to the internal 1.1V reference
    ADMUX = (1<<REFS0) | (1<<MUX3) | (1<<MUX2) | (1<<MUX1);
    delay(2); // Wait for Vref to settle
    ADCSRA |= (1<<ADSC); // Start conversion
    while (bit_is_set(ADCSRA,ADSC)); // measuring
    unsigned int result = ADC;

    //custom scale factor, processor specific
    result = 1125300UL / (unsigned long)result; // Calculate Vcc (in mV); 1125300 = 1.1*1024*1000
    mV_act_ = result; // Vcc in millivolts
}

uint8_t BatteryState::getCurrent_mV()
{
    return mV_act_;
}

uint8_t BatteryState::computePercent()
{
    int8_t percent = 0;
    if(mV_act_ < mV_0prc_) {
        percent = 0;
    }
    else if(mV_act_ > mV_100prc_) {
        percent = 100;
    }
    else{
       percent = map(mV_act_, mV_0prc_, mV_100prc_, 0, 100);
    }

    // PRINTD1F("batvin=["); PRINTD1(mV_act);PRINTLND1F("]");
    // PRINTD1F("batprcnt=["); PRINTD1(percent);PRINTLND1F("]");

    return percent;
}

void BatteryState::setOmmitCycles(uint8_t ommitCycles)
{
    ommitCycles_ = ommitCycles;
}

uint8_t BatteryState::getOmmitCycles()
{
    return ommitCycles_;
}
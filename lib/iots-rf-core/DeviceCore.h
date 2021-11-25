#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <SPIFlash.h>
#include "DebugLogger.h"
#include "DeviceClass.h"
#include "RFM69_ATC.h"
#include "UuidConfigEEPROM.h"
#include "RadioConfigEEPROM.h"
#include "LowPowerWrapper.h"
#include "BatteryState.h"
#include "Buffer.h"
#include "Action.h"

#define LED_PIN 9
#define FLASH_SS 8

class DeviceCore
{
public:
    DeviceCore(DeviceClass deviceClass);

    virtual void setup();
    virtual void loop();

    static bool IsExtInterrupt;

protected:
    // Configuration
    void readConfiguration(unsigned int delay = 10000U);

    // Initialization
    void initExtFlash();
    void initRadio(uint8_t nodeId);

    // Messages
    bool registerNode();
    bool informNodeAlive();
    virtual bool sendState() = 0;
    bool getCommand();

    bool sendBufferToRadio(bool ack = false);
    bool loadBufferFromRadio();
    bool sendBufferToSerial();
    bool loadBufferFromSerial();
    bool ackCheck(bool rc);
    bool loadRadioDataToBuffer();

    void setInformNodeAlivePeriodSec(uint16_t period);
    uint16_t getInformNodeAlivePeriodSec();
    
    void deepSleepDelay(unsigned int delay = 0);
    void deepSleepForewerAndWakeInt(uint8_t pin_, uint8_t mode);

    bool verifyBaseFrame(ActionType actionType, ActionDirection actionDirection);
    
protected:
    DeviceClass deviceClass_;
    RFM69_ATC radio_;
    UuidConfigEEPROM uuidConfig_;
    RadioConfigEEPROM radioConfig_;
    char serialTerminator_ = '\n';
    SPIFlash flash_ = SPIFlash(FLASH_SS, 0xEF30);
    BatteryState batteryState_;
    Buffer buffer_;
    uint16_t informNodeAlivePeriodSec_ = 15;
    bool nodeRegistered_ = false;
};
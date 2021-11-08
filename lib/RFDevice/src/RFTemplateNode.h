#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <SPIFlash.h>
#include "DebugLogger.h"
#include "RFM69_ATC.h"
#include "LowPowerWrapper.h"

#define LED_PIN 9
#define FLASH_SS 8

class RFTemplateNode
{
public:
    RFTemplateNode();

    enum class WakeupMode{
        Periodical,
        Interrupt
    };

    virtual void setup();
    virtual void loop();

protected:
    RFM69_ATC &getRadio();

    void setGatewayId(uint8_t gatewayId);
    void setPeriodicalWakeupMode(uint32_t delay = 10000U);
    void setInterruptedWakeupMode(uint8_t wakeupPin = 3);

    virtual void initExtFlash();
    virtual void sleepExtFlash();
    virtual void initRadio() = 0;
    virtual void initAdcReference();
    virtual void initBatteryADC();

    virtual void work();

    virtual void sendRadioBuffer();
    virtual void checkRadioBuffer();

    virtual uint8_t buildNodeUniqueIdByte();
    virtual void buildNodeUniqueIdHex(char *buffer);
    virtual void appendSendBufferText(const char *data, bool lastItem = false);
    virtual void appendSendBufferInt(int value, bool lastItem = false);
    virtual void appendSendBufferFloat(double value, bool lastItem = false);
    virtual void appendSendBufferDelimeter();
    virtual uint16_t getBatteryADC();
    virtual uint8_t getBtteryPercent();
    

private:
    WakeupMode mode_ = WakeupMode::Periodical;   // Default
    unsigned long cycle_ = 0;

    static constexpr uint8_t SendBufferSize = 100;
    char sendBuffer_[SendBufferSize];

    uint8_t gatewayId_ = 1;


    uint8_t batteryADC_pin = A0;
    uint32_t periodicalDelay_ = 10000U;
    uint8_t interruptWakeup_pin_ = 3;

    RFM69_ATC radio_;
    SPIFlash flash_ = SPIFlash(FLASH_SS, 0xEF30);
    bool isFlashInitialized = false;
};
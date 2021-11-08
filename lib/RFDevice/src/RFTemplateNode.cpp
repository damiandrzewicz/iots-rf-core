#include "RFTemplateNode.h"

RFTemplateNode::RFTemplateNode()
{
}

void RFTemplateNode::setup()
{
    initExtFlash();
    initRadio();
    sleepExtFlash();
    initAdcReference();
    initBatteryADC();
    
    PRINTLND1F("basic setup done!");
}

void wakeUp(){} // Make 'attachInterrupt' happy

void RFTemplateNode::loop()
{
    work();
    sendRadioBuffer();

    PRINTLND1F("work done!");
    Serial.flush();

    if(mode_ == WakeupMode::Periodical)
    {
        // Periodical wakeup mode
        getRadio().sleep();
        LowPowerWrp.DeepSleep(periodicalDelay_);
    }
    else if(mode_ == WakeupMode::Interrupt)
    {
        // Interrupt wakeup mode
        getRadio().sleep();
        attachInterrupt(digitalPinToInterrupt(interruptWakeup_pin_), wakeUp, LOW);
        LowPowerWrp.DeepSleepForever();
        detachInterrupt(0); 
    }
    else
    {
        PRINTLND1("unsupported mode!");
    }
}

void RFTemplateNode::work()
{
    memset(sendBuffer_, 0, sizeof(sendBuffer_)); // Clear buffer

    cycle_++;  // Increase cycle indicator every loop
    PRINTD1F("work..., cycle=[");PRINTD1(cycle_);PRINTLND1F("]");

    // Read batter percent
    auto batteryPercent = getBtteryPercent();
    PRINTD1F("battpr=[");PRINTLND1F(batteryPercent);PRINTLND1F("]");
    appendSendBufferInt(batteryPercent);
}

void RFTemplateNode::sendRadioBuffer()
{
    PRINTD1F("--->air_out: [");PRINTD1(sendBuffer_);PRINTLND1F("]");

    if(radio_.sendWithRetry(gatewayId_, sendBuffer_, strlen(sendBuffer_)))
    {
        PRINTLND1F("radio sent!");
        if (radio_.ACKRequested())
        {
            radio_.sendACK();
            PRINTLND1F("ACK sent!");
        }
    }
    else
    {
        PRINTLND1F("send failed!");
    }
}

void RFTemplateNode::checkRadioBuffer()
{
    // Not implemented!
}

RFM69_ATC &RFTemplateNode::getRadio()
{
    return radio_;
}


void RFTemplateNode::setGatewayId(uint8_t gatewayId)
{
    gatewayId_ = gatewayId;
}


void RFTemplateNode::setPeriodicalWakeupMode(uint32_t delay)
{
    mode_ = WakeupMode::Periodical;
    periodicalDelay_ = delay;
}

void RFTemplateNode::setInterruptedWakeupMode(uint8_t interruptWakeupPin)
{
    mode_ = WakeupMode::Interrupt;
    interruptWakeup_pin_ = interruptWakeupPin;
}

void RFTemplateNode::initExtFlash()
{
  // Initialize connected flash
  if (flash_.initialize()){
    PRINTLND1F("SPI Flash Init OK. Unique MAC = [");
    isFlashInitialized = true;
  }
  else{
    PRINTLND1F("SPI Flash MEM not found (is chip soldered?)...");
  }
}

void RFTemplateNode::sleepExtFlash()
{
    if (isFlashInitialized)
    {
        flash_.sleep();
    }
}

void RFTemplateNode::initAdcReference()
{
    analogReference(INTERNAL);
}

void RFTemplateNode::initBatteryADC()
{
    pinMode(batteryADC_pin, INPUT);
}

uint8_t RFTemplateNode::buildNodeUniqueIdByte()
{
    flash_.readUniqueId();
    uint16_t uniqueId = 0;
    for (byte i=0;i<8;i++){
        uniqueId += flash_.UNIQUEID[i];
    }
    return static_cast<uint8_t>(uniqueId/16);
}

void RFTemplateNode::buildNodeUniqueIdHex(char *buffer)
{
    flash_.readUniqueId();
    sprintf(buffer, "%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X",
    flash_.UNIQUEID[0],
    flash_.UNIQUEID[1],
    flash_.UNIQUEID[2],
    flash_.UNIQUEID[3],
    flash_.UNIQUEID[4],
    flash_.UNIQUEID[5],
    flash_.UNIQUEID[6],
    flash_.UNIQUEID[7]
  );
}

void RFTemplateNode::appendSendBufferText(const char *data, bool lastItem)
{
    strcat(sendBuffer_, data);
    if(!lastItem){ appendSendBufferDelimeter(); }
}

void RFTemplateNode::appendSendBufferInt(int value, bool lastItem)
{
    snprintf(sendBuffer_ + strlen(sendBuffer_), SendBufferSize - strlen(sendBuffer_), "%d", value);
    if(!lastItem){ appendSendBufferDelimeter(); }
}

void RFTemplateNode::appendSendBufferFloat(double value, bool lastItem)
{
    snprintf(sendBuffer_ + strlen(sendBuffer_), SendBufferSize - strlen(sendBuffer_), "%.2f", value);
    if(!lastItem){ appendSendBufferDelimeter(); }
}

void RFTemplateNode::appendSendBufferDelimeter()
{
    strcat(sendBuffer_, "|");
}

uint16_t RFTemplateNode::getBatteryADC()
{
    uint16_t temp = 0;
    uint8_t repeats = 8;
    for(uint8_t i = 0; i < repeats; i++){
        temp += analogRead(batteryADC_pin);
    }
    return temp / repeats;
}

uint8_t RFTemplateNode::getBtteryPercent()
{
    uint16_t adc = getBatteryADC();
    int8_t percent = map(adc, 650, 1015, 0, 100);
    if(percent > 100) percent = 100;
    else if(percent < 0) percent = 0;
    PRINTD1F("batadc=["); PRINTD1(adc);PRINTLND1F("]");
    PRINTD1F("batprcnt=["); PRINTD1(percent);PRINTLND1F("]");

    return percent;
}
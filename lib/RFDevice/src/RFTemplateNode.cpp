#include "RFTemplateNode.h"

bool RFTemplateNode::IsInterrupt = false;

uint8_t RFTemplateNode::BatteryState::computePercent()
{
    int8_t percent = map(mV_act, mV_0prc, mV_100prc, 0, 100);
    if(percent > 100) percent = 100;
    else if(percent < 0) percent = 0;
    PRINTD1F("batvin=["); PRINTD1(mV_act);PRINTLND1F("]");
    PRINTD1F("batprcnt=["); PRINTD1(percent);PRINTLND1F("]");

    return percent;
}

RFTemplateNode::RFTemplateNode()
{
}

void RFTemplateNode::setup()
{
    initExtFlash();
    initRadio();
    sleepExtFlash();
    
    PRINTLND1F("basic setup done!");
}

void wakeUp(){
    RFTemplateNode::IsInterrupt = true;
} // Make 'attachInterrupt' happy

void RFTemplateNode::loop()
{
    clearSendBuffer();
    increaseCycleCounter();

    work();

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
        attachInterrupt(digitalPinToInterrupt(interruptWakeup_pin_), wakeUp, interruptMode_);
        IsInterrupt = false;    // Clear interrupt flag
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

void RFTemplateNode::setInterruptedWakeupMode(uint8_t wakeupPin, uint8_t mode)
{
    mode_ = WakeupMode::Interrupt;
    interruptWakeup_pin_ = wakeupPin;
    interruptMode_ = mode;
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

RFTemplateNode::BatteryState &RFTemplateNode::getBatteryState()
{
    return batteryState_;
}

unsigned int RFTemplateNode::readVin()
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
  return result; // Vcc in millivolts
}

void RFTemplateNode::readBattery()
{
    batteryState_.mV_act = readVin();
    appendSendBufferInt(batteryState_.computePercent());
}

void RFTemplateNode::clearSendBuffer()
{
    memset(sendBuffer_, 0, sizeof(sendBuffer_)); // Clear buffer
}

void RFTemplateNode::increaseCycleCounter()
{
    cycle_++;  // Increase cycle indicator every loop
    PRINTD1F("work..., cycle=[");PRINTD1(cycle_);PRINTLND1F("]");
}
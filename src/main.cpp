#include <Arduino.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include "RFM69_ATC.h"
#include "PrivateConig.h"
#include <SPIFlash.h>
#include "LowPowerWrapper.h"
#include "uptime.h"

#include "DebugLogger.h"
#include "RFBaseNode.h"

RFBaseNode node;


void setup() 
{     
  Serial.begin(38400);
  node.setup();
}

void loop() 
{
  node.loop();
}
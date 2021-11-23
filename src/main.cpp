#include <Arduino.h>
#include "DeviceExample.h"

DeviceExample node;

void setup() 
{     
  Debug = 1;
  Serial.begin(38400);
  node.setup();
}

void loop() 
{
  node.loop();
}
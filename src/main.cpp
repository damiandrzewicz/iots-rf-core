#include <Arduino.h>
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
#pragma once

#include <Arduino.h>

#define DEGUB 1

#define PRINTD1(arg1) if(DEGUB == 1) Serial.print(arg1)
#define PRINTD1F(arg1) if(DEGUB == 1) Serial.print(F(arg1))
#define PRINTD2(arg1, arg2) if(DEGUB == 1) Serial.print(arg1, arg2)
#define PRINTD2F(arg1, arg2) if(DEGUB == 1) Serial.print(F(arg1), arg2)
#define PRINTLND1(arg1) if(DEGUB == 1) Serial.println(arg1)
#define PRINTLND1F(arg1) if(DEGUB == 1) Serial.println(arg1)
#define PRINTLND2(arg1,arg2) if(DEGUB == 1) Serial.println(arg1,arg2)
#define PRINTLND2F(arg1,arg2) if(DEGUB == 1) Serial.println(F(arg1),arg2)
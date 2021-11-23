#pragma once

#include <Arduino.h>

extern bool Debug;

#if defined (ARDUINO_ARCH_ESP8266) || defined(ESP32)

#else
    #define PRINTD1(arg1) if(Debug == 1) Serial.print(arg1)
    #define PRINTD1F(arg1) if(Debug == 1) Serial.print(F(arg1))
    #define PRINTD2(arg1, arg2) if(Debug == 1) Serial.print(arg1, arg2)
    #define PRINTD2F(arg1, arg2) if(Debug == 1) Serial.print(F(arg1), arg2)
    #define PRINTLND1(arg1) if(Debug == 1) Serial.println(arg1)
    #define PRINTLND1F(arg1) if(Debug == 1) Serial.println(arg1)
    #define PRINTLND2(arg1,arg2) if(Debug == 1) Serial.println(arg1,arg2)
    #define PRINTLND2F(arg1,arg2) if(Debug == 1) Serial.println(F(arg1),arg2)
#endif
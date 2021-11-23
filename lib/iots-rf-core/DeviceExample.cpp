#include "DeviceExample.h"

DeviceExample::DeviceExample()
    : DeviceCore(DeviceClass::TemplateDevice)
{

}

void DeviceExample::loop()
{
    Serial.println("lp");
    deepSleepDelay();
}

bool DeviceExample::sendState()
{
    return true;
}
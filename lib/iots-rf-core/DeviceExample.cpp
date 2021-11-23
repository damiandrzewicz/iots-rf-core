#include "DeviceExample.h"

DeviceExample::DeviceExample()
    : DeviceCore(DeviceClass::TemplateDevice)
{

}

void DeviceExample::loop()
{
    Serial.println("lp");
    informNodeAlive();
    deepSleepDelay(getInformNodeAlivePeriodSec() * 1000);
}

bool DeviceExample::sendState()
{
    return true;
}
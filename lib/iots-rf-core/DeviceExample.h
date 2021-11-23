#pragma once

#include "DeviceCore.h"

class DeviceExample : public DeviceCore
{
public:
    DeviceExample();
    
    virtual void loop() override;
    virtual bool sendState() override;

private:

};
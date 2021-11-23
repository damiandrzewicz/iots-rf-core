#pragma once
#include <Arduino.h>
#include "IConfigEEPROM.h"

class RadioConfigEEPROM : public IConfigEEPROM
{
public:
    struct Data{
        Data();
        bool isEmpty();
        void setEmpty();
        

        uint8_t networkId;
        uint8_t gatewayId;
        unsigned long customFrequency;
        char encryptKey[17];
        int8_t powerLevel;
    };

    RadioConfigEEPROM() = default;

    Data &data();
    void save() override;
    void read() override;
    void clear() override;

    uint8_t getUnregisteredNodeId();

private:
    int address_ = 0;
    Data data_;
    
};
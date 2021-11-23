#pragma once
#include <Arduino.h>
#include "IConfigEEPROM.h"

class UuidConfigEEPROM : public IConfigEEPROM
{
public:
    struct Data{
        Data();
        bool isEmpty();
        void setEmpty();
        char uuid[7];
    };

    UuidConfigEEPROM() = default;

    Data &data();
    void save() override;
    void read() override;
    void clear() override; 

private:
    int address_ = 50;
    Data data_;
};
#include "RadioConfigEEPROM.h"
#include <EEPROM.h>

RadioConfigEEPROM::Data::Data()
{
    setEmpty();
}

bool RadioConfigEEPROM::Data::isEmpty()
{
    return networkId == 0xff;
}

void RadioConfigEEPROM::Data::setEmpty()
{
    networkId = 0xff;
}



RadioConfigEEPROM::Data &RadioConfigEEPROM::data()
{
    return data_;
}

void RadioConfigEEPROM::save()
{
    Serial.println(F("writing eeprom..."));
    EEPROM.put(address_, data_);
}

void RadioConfigEEPROM::read()
{
    Serial.println(F("reading eprom..."));
    EEPROM.get(address_, data_);

    if(data_.isEmpty()) {
        // EEPROM was empty -> initialize default
        data_.gatewayId = 1;
        data_.networkId = 100;
        data_.customFrequency = 0;
        data_.powerLevel = 0;
        strcpy_P(data_.encryptKey, PSTR("sampleEncryptKey"));
        Serial.println(F("no data, reading default..."));
    }
}

void RadioConfigEEPROM::clear()
{
    uint8_t size = sizeof(data_);
    Serial.print(F("clearing radio"));Serial.print(size);Serial.println(F("eeprom bytes..."));
    for (unsigned int i = address_; i < address_ + sizeof(data_); i++)
    {
        EEPROM.write(i, 0xff);
    }
}

uint8_t RadioConfigEEPROM::getUnregisteredNodeId()
{
    return random(2, 6);
}
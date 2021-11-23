#include "UuidConfigEEPROM.h"
#include <EEPROM.h>

UuidConfigEEPROM::Data::Data()
{
    setEmpty();
}

bool UuidConfigEEPROM::Data::isEmpty()
{
    return uuid[0] == -1;
}

void UuidConfigEEPROM::Data::setEmpty()
{
    memset(uuid, 0xff,sizeof(uuid));
}

UuidConfigEEPROM::Data &UuidConfigEEPROM::data()
{
    return data_;
}

void UuidConfigEEPROM::save()
{
    //Serial.println(F("writing eeprom..."));
    EEPROM.put(address_, data_);
}

void UuidConfigEEPROM::read()
{
    //Serial.println(F("reading eprom..."));
    EEPROM.get(address_, data_);
    Serial.print(F("data_ uuid "));Serial.print(data_.uuid[0], DEC);
}

void UuidConfigEEPROM::clear()
{
    //uint8_t size = sizeof(data_);
    //Serial.print(F("clearing uuid "));Serial.print(size);Serial.println(F(" eeprom bytes..."));
    for (unsigned int i = address_; i < address_ + sizeof(data_); i++)
    {
        EEPROM.write(i, 0xff);
    }
}
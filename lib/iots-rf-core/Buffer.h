#pragma once

#include <Arduino.h>

class Buffer
{
public:
    Buffer();

    char *data();
    uint8_t size();
    void appendText(const char *data, bool lastItem = false);
    void appendInt(long value, bool lastItem = false);
    void appendFloat(double value, uint8_t size = 4, uint8_t prec = 2, bool lastItem = false);
    void appendDelimeter();
    void clear();

    static constexpr uint8_t Size = 100;

private:
    char buffer_[Size];
};
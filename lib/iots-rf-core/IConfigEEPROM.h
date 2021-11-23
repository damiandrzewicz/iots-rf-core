#pragma once

class IConfigEEPROM
{
public:
    virtual ~IConfigEEPROM() = default;

    virtual void save() = 0;
    virtual void read() = 0;
    virtual void clear() = 0;
};  
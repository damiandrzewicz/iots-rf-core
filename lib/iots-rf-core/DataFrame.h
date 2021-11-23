#pragma once

#include <Arduino.h>
#include "Buffer.h"

template<class Req, class Res>
class DataFrame
{
public:
    virtual bool build(Buffer &buff, const Req &request) = 0;
    virtual bool build(Buffer &buff, const Res &response) = 0;
    virtual bool parse(Req &request, Buffer &buff) = 0;
    virtual bool parse(Res &response, Buffer &buff) = 0;
};
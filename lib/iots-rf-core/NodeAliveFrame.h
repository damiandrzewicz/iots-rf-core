#pragma once
#include <Arduino.h>
#include "DataFrame.h"
#include "DeviceClass.h"

struct NodeAliveRequest{
    uint8_t batteryPercent;
};

struct NodeAliveResponse : NodeResponse{
    
};

class NodeAliveFrame : public DataFrame<NodeAliveRequest, NodeAliveResponse>
{
public:
    virtual bool build(Buffer &buff, const NodeAliveRequest &request) override{
        buff.clear();
        buff.appendInt(static_cast<int>(ActionType::Alive));
        buff.appendInt(static_cast<int>(ActionDirection::Request));
        buff.appendInt(request.batteryPercent, true);
        return true;
    }

    virtual bool build(Buffer &buff, const NodeAliveResponse &response) override{
        return true;
    }

    virtual bool parse(NodeAliveRequest &request, Buffer &buff) override{
        int batteryPercent;
        
        if( 1 != sscanf(buff.data(), "%*d|%*d|%d", &batteryPercent)){
            return false;
        }

        request.batteryPercent = (uint8_t)batteryPercent;
        return true;
    }

    virtual bool parse(NodeAliveResponse &response, Buffer &buff) override{
        return true;
    }


private:
};
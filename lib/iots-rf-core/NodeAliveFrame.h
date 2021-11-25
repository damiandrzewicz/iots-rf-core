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
        buff.clear();
        buff.appendInt(static_cast<int>(ActionType::Alive));
        buff.appendInt(static_cast<int>(ActionDirection::Response));
        buff.appendInt(static_cast<int>(response.error), true);
        return true;
    }

    virtual bool parse(NodeAliveRequest &request, Buffer &buff) override{
        char *data = buff.data();
        parseIgnoreBaseFrame(data);

        char *batteryPercent = strtok(NULL, ActionDelim);
        if(!batteryPercent){
            return false;
        }

        request.batteryPercent = atoi(batteryPercent);
        return true;
    }

    virtual bool parse(NodeAliveResponse &response, Buffer &buff) override{
        char *data = buff.data();
        auto ret = fillResponseWithActionError(data, response);
        if(ret == -1){return false;}
        else if(ret){return true;}
        return true;
    }


private:
};
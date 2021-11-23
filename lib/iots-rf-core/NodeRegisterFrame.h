#pragma once
#include <Arduino.h>
#include "DataFrame.h"
#include "DeviceClass.h"

struct NodeRegisterRequest{
    char uuid[7];
    DeviceClass deviceClass;
    uint16_t sleepTime;
};

struct NodeRegisterResponse{
    uint8_t nodeId;
};

class NodeRegisterFrame : public DataFrame<NodeRegisterRequest, NodeRegisterResponse>
{
public:
    virtual bool build(Buffer &buff, const NodeRegisterRequest &request) override{
        buff.clear();
        buff.appendInt(static_cast<int>(ActionType::Register));
        buff.appendInt(static_cast<int>(ActionDirection::Request));
        buff.appendText(request.uuid);
        buff.appendInt(static_cast<int>(request.deviceClass));
        buff.appendInt(request.sleepTime, true);
        return true;
    }

    virtual bool build(Buffer &buff, const NodeRegisterResponse &response) override{
        buff.clear();
        buff.appendInt(static_cast<int>(ActionType::Register));
        buff.appendInt(static_cast<int>(ActionDirection::Response));
        buff.appendInt(response.nodeId, true);
        return true;
    }

    virtual bool parse(NodeRegisterRequest &request, Buffer &buff) override{
        int deviceClass;
        int sleepTime;
        
        if( 3 != sscanf(buff.data(), "%*d|%*d|%6s|%d|%d", request.uuid, &deviceClass, &sleepTime)){
            return false;
        }

        request.deviceClass = static_cast<DeviceClass>(deviceClass);
        request.sleepTime = sleepTime;
        return true;
    }

    virtual bool parse(NodeRegisterResponse &response, Buffer &buff) override{
        int nodeId;
        if( 1 != sscanf(buff.data(), "%*d|%*d|%d", &nodeId)){
            return false;
        }
        response.nodeId = nodeId;
        return true;
    }


private:
};
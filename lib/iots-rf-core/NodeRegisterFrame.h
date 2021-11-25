#pragma once
#include <Arduino.h>
#include "DataFrame.h"
#include "DeviceClass.h"

struct NodeRegisterRequest{
    char uuid[7];
    DeviceClass deviceClass;
    uint16_t sleepTime;
};

struct NodeRegisterResponse : NodeResponse{
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

        bool isError = response.error != ActionError::Ok;
        buff.appendInt(static_cast<int>(response.error), isError);
        
        if(!isError){
            buff.appendInt(response.nodeId, true);
        }
        return true;
    }

    virtual bool parse(NodeRegisterRequest &request, Buffer &buff) override{
        char *data = buff.data();
        parseIgnoreBaseFrame(data);

        char *uuid = strtok(NULL, ActionDelim);
        char *deviceClass = strtok(NULL, ActionDelim);
        char *sleepTime = strtok(NULL, ActionDelim);

        if(!uuid || !deviceClass || !sleepTime){
            return false;
        }

        strncpy(request.uuid, uuid, sizeof(request.uuid));
        request.deviceClass = static_cast<DeviceClass>(atoi(deviceClass));
        request.sleepTime = atoi(sleepTime);
        return true;
    }

    virtual bool parse(NodeRegisterResponse &response, Buffer &buff) override{
        char *data = buff.data();
        auto ret = fillResponseWithActionError(data, response);
        if(ret == -1){return false;}
        else if(ret){return true;}

        // Get rest of arguments, no error
        char *sNodeId = strtok(NULL, ActionDelim);
        if(!sNodeId){return false;}
        response.nodeId = atoi(sNodeId);
        return true;
    }

private:
};
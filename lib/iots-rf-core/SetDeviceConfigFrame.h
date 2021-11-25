#pragma once
#include <Arduino.h>
#include "DataFrame.h"

struct SetDeviceConfigRequest{
    char *uuid;
    uint8_t networkId;
    uint8_t gatewayId;
    unsigned long customFrequency;
    char *encryptKey;
    int8_t powerLevel;
};

struct SetDeviceConfigResponse : NodeResponse{

};

class SetDeviceConfigFrame : public DataFrame<SetDeviceConfigRequest, SetDeviceConfigResponse>
{
public:
    virtual bool build(Buffer &buff, const SetDeviceConfigRequest &request) override{
        buff.clear();
        buff.appendInt(static_cast<int>(ActionType::GetDeviceConfig));
        buff.appendInt(static_cast<int>(ActionDirection::Request));
        buff.appendText(request.uuid);
        buff.appendInt(request.networkId);
        buff.appendInt(request.gatewayId);
        buff.appendText(request.encryptKey);
        buff.appendInt(request.powerLevel);
        buff.appendInt(request.customFrequency);
        return true;
    }

    virtual bool build(Buffer &buff, const SetDeviceConfigResponse &response) override{
        buff.clear();
        buff.appendInt(static_cast<int>(ActionType::GetDeviceConfig));
        buff.appendInt(static_cast<int>(ActionDirection::Response));

        bool isError = response.error != ActionError::Ok;
        buff.appendInt(static_cast<int>(response.error), isError);

        return true;
    }

    virtual bool parse(SetDeviceConfigRequest &request, Buffer &buff) override{
        char *data = buff.data();
        parseIgnoreBaseFrame(data);

        char *temp = NULL;

        temp = strtok(NULL, ActionDelim);
        if(!temp){ return false; }
        request.uuid = temp;

        temp = strtok(NULL, ActionDelim);
        if(!temp){ return false; }
        request.networkId = atoi(temp);

        temp = strtok(NULL, ActionDelim);
        if(!temp){ return false; }
        request.gatewayId = atoi(temp);

        temp = strtok(NULL, ActionDelim);
        if(!temp){ return false; }
        request.encryptKey = temp;

        temp = strtok(NULL, ActionDelim);
        if(!temp){ return false; }
        request.powerLevel = atoi(temp);

        temp = strtok(NULL, ActionDelim);
        if(!temp){ return false; }
        request.customFrequency = atol(temp);

        return true;
    }

    virtual bool parse(SetDeviceConfigResponse &response, Buffer &buff) override{
        char *data = buff.data();
        auto ret = fillResponseWithActionError(data, response);
        if(ret == -1){return false;}
        else if(ret){return true;}
        return true;
    }

private:
};
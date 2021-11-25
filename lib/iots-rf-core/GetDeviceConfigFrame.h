#pragma once
#include <Arduino.h>
#include "DataFrame.h"

struct GetDeviceConfigRequest{
};

struct GetDeviceConfigResponse : NodeResponse{
    char *uuid;
    uint8_t networkId;
    uint8_t gatewayId;
    unsigned long customFrequency;
    char *encryptKey;
    int8_t powerLevel;
};

class GetDeviceConfigFrame : public DataFrame<GetDeviceConfigRequest, GetDeviceConfigResponse>
{
public:
    virtual bool build(Buffer &buff, const GetDeviceConfigRequest &request) override{
        buff.clear();
        buff.appendInt(static_cast<int>(ActionType::GetDeviceConfig));
        buff.appendInt(static_cast<int>(ActionDirection::Request));
        return true;
    }

    virtual bool build(Buffer &buff, const GetDeviceConfigResponse &response) override{
        buff.clear();
        buff.appendInt(static_cast<int>(ActionType::GetDeviceConfig));
        buff.appendInt(static_cast<int>(ActionDirection::Response));

        bool isError = response.error != ActionError::Ok;
        buff.appendInt(static_cast<int>(response.error), isError);

        if(!isError){
            buff.appendText(response.uuid);
            buff.appendInt(response.networkId);
            buff.appendInt(response.gatewayId);
            buff.appendText(response.encryptKey);
            buff.appendInt(response.powerLevel);
            buff.appendInt(response.customFrequency);
        }
        return true;
    }

    virtual bool parse(GetDeviceConfigRequest &request, Buffer &buff) override{
        char *data = buff.data();
        parseIgnoreBaseFrame(data);
        return true;
    }

    virtual bool parse(GetDeviceConfigResponse &response, Buffer &buff) override{
        char *data = buff.data();
        auto ret = fillResponseWithActionError(data, response);
        if(ret == -1){return false;}
        else if(ret){return true;}

        char *temp = NULL;

        temp = strtok(NULL, ActionDelim);
        if(!temp){ return false; }
        response.uuid = temp;

        temp = strtok(NULL, ActionDelim);
        if(!temp){ return false; }
        response.networkId = atoi(temp);

        temp = strtok(NULL, ActionDelim);
        if(!temp){ return false; }
        response.gatewayId = atoi(temp);

        temp = strtok(NULL, ActionDelim);
        if(!temp){ return false; }
        response.encryptKey = temp;

        temp = strtok(NULL, ActionDelim);
        if(!temp){ return false; }
        response.powerLevel = atoi(temp);

        temp = strtok(NULL, ActionDelim);
        if(!temp){ return false; }
        response.customFrequency = atol(temp);

        return true;
    }

private:
};
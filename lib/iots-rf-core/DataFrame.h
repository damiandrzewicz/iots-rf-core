#pragma once

#include <Arduino.h>
#include "Buffer.h"
#include "Action.h"

struct NodeResponse{
    ActionError error;
};

template<class Req, class Res>
class DataFrame
{
public:
    virtual bool build(Buffer &buff, const Req &request) = 0;
    virtual bool build(Buffer &buff, const Res &response) = 0;
    virtual bool parse(Req &request, Buffer &buff) = 0;
    virtual bool parse(Res &response, Buffer &buff) = 0;

protected:
    virtual bool parseIgnoreBaseFrame(char *data){
        strtok(data, ActionDelim);  // Ignore ActionType
        strtok(NULL, ActionDelim);  // Ignore ActionDirection
        return true;
    }

    virtual bool parseResponseActionError(ActionError *actionError){
        char *sActionError = strtok(NULL, ActionDelim);
        if(!sActionError){return false;}
        *actionError = static_cast<ActionError>(atoi(sActionError));
        return true;
    }

    virtual int fillResponseWithActionError(char *data, NodeResponse &response){
        if(!parseIgnoreBaseFrame(data)){
            return -1;
        }
        
        ActionError actionError;
        if(!parseResponseActionError(&actionError)){
            return -1;
        }
        response.error = actionError;
        return actionError == ActionError::Ok ? 0 : 1;
    }
};
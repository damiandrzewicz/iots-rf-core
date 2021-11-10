#pragma once

#include "RFTemplateNode.h"

class RFBaseNode : public RFTemplateNode
{
public:
    RFBaseNode() = default;

    virtual void setup() override;

protected:
    virtual void work() override;
    virtual void initRadio() override;
};
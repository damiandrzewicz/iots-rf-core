#pragma once

#include "RFTemplateNode.h"

class RFExampleNode : public RFTemplateNode
{
public:
    RFExampleNode() = default;

protected:
    void initRadio() override;
};
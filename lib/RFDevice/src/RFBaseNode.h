#pragma once

#include "RFTemplateNode.h"

class RFBaseNode : public RFTemplateNode
{
public:
    RFBaseNode() = default;

protected:
    void initRadio() override;
};
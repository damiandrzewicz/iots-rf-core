#include "RFBaseNode.h"
#include <Wire.h>

void RFBaseNode::initRadio()
{
    auto nodeID = buildNodeUniqueIdByte();
    auto networkID = 101;

    if(getRadio().initialize(RF69_868MHZ, nodeID, networkID))
    {
        PRINTD1F("initialized radio... nodeId: [");
        PRINTD2(nodeID, DEC);
        PRINTD1F("], networkId:[");
        PRINTD2(networkID, DEC);
        PRINTLND1F("]");

        getRadio().setHighPower(); //must include this only for RFM69HW/HCW!
        getRadio().encrypt("sampleEncryptKey");
        getRadio().setPowerLevel(0);
        getRadio().setFrequency(869000000);
        getRadio().sleep();
        PRINTLND1F("radio started in sleep mode!");
    }
    else{
        PRINTLND1F("radio initialize failed!");
    }
}
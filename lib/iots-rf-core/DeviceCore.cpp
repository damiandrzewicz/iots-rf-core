#include "DeviceCore.h"
#include "BaseFrame.h"
#include "NodeRegisterFrame.h"
#include "NodeAliveFrame.h"
#include "SetDeviceConfigFrame.h"
#include "GetDeviceConfigFrame.h"

bool DeviceCore::IsExtInterrupt = false;

DeviceCore::DeviceCore(DeviceClass deviceClass)
{
    deviceClass_ = deviceClass;
}

void DeviceCore::setup()
{
    readConfiguration();
    initExtFlash();
    
    PRINTD1F("DC setup ok!");
}

void DeviceCore::loop()
{
    registerNode();
}

void DeviceCore::readConfiguration(unsigned int delay)
{
    uuidConfig_.read();
    bool waitUntilUUID = true;
    Serial.print(F("waitUntilUUID:"));Serial.println(waitUntilUUID);

    radioConfig_.read();

    uint16_t period = 15000U;
    unsigned long time_now = millis();

    BaseFrame baseFrame;
    while(waitUntilUUID || (millis() < time_now + period))
    {
         if(loadBufferFromSerial()){
            BaseFrameData bfd;
            if(!baseFrame.parse(buffer_, bfd)){
                continue;
            }

            if(bfd.actionDirection == ActionDirection::Request){

            }

            if(bfd.actionType == ActionType::GetDeviceConfig){
                GetDeviceConfigFrame devCfFr;
                GetDeviceConfigResponse response;
                response.uuid = uuidConfig_.data().uuid;
                response.networkId = radioConfig_.data().networkId;
                response.gatewayId = radioConfig_.data().gatewayId;
                response.encryptKey = radioConfig_.data().encryptKey;
                response.powerLevel = radioConfig_.data().powerLevel;
                response.customFrequency = radioConfig_.data().customFrequency;
                response.error = ActionError::Ok;

                devCfFr.build(buffer_, response);

                sendBufferToSerial();
            }
            else if(bfd.actionType == ActionType::SetDeviceConfig){
                SetDeviceConfigFrame devCfFr;
                SetDeviceConfigRequest request;
                SetDeviceConfigResponse response;

                if(!devCfFr.parse(request, buffer_)){
                    response.error = ActionError::ParseFrameError;
                }
                else{
                    strncpy(uuidConfig_.data().uuid, request.uuid, sizeof(uuidConfig_.data().uuid));
                    radioConfig_.data().networkId = request.networkId;
                    radioConfig_.data().gatewayId = request.gatewayId;
                    strncpy(radioConfig_.data().encryptKey, request.encryptKey, sizeof(radioConfig_.data().encryptKey));
                    radioConfig_.data().powerLevel = request.powerLevel;
                    radioConfig_.data().customFrequency = request.customFrequency;

                    uuidConfig_.save();
                    radioConfig_.save();

                    response.error = ActionError::Ok;
                }

                devCfFr.build(buffer_, response);
                sendBufferToSerial();
            }
    //         Action action = dfp.parseAction(buffer_);
    //         if(action == Action::ResetToDefault){
    //             radioConfig_.clear();
    //             radioConfig_.read();
    //         }
    //         else if(action == Action::GetUUID){
    //             if(dfp.buildUUIDFrame(buffer_, uuidConfig_.data())){
    //                 sendBufferToSerial();
    //             }
    //             else{
    //                 sendConfigError(Error::BuildFrameError);
    //             }
    //         }
    //         else if(action == Action::SetUUID){
    //             if(dfp.parseUUIDFrame(uuidConfig_.data())){
    //                 uuidConfig_.save();
    //             }
    //             else{
    //                 sendConfigError(Error::ParseFrameError);
    //             }
    //         }
    //         else if(action == Action::GetRadioConfig){
    //             if(dfp.buildRadioConfigFrame(buffer_,radioConfig_.data())){
    //                 sendBufferToSerial();
    //             }
    //             else{
    //                 sendConfigError(Error::BuildFrameError);
    //             }
    //         }
    //         else if(action == Action::SetRadioConfig){
    //             if(dfp.parseRadioConfigFrame(radioConfig_.data())){
    //                 radioConfig_.save();
    //             }
    //             else{
    //                 sendConfigError(Error::ParseFrameError);
    //             }
    //         }
    //         else if(action == Action::Unsupported){
    //             sendConfigError(Error::ActionUnsupported);
    //         }
         }

        waitUntilUUID = uuidConfig_.data().isEmpty();
    }

    Serial.print(F("config done!"));
}

void DeviceCore::initExtFlash()
{
    // Initialize connected flash
    if (flash_.initialize()){
        Serial.println("SPI Flash Init OK. Unique MAC = [");
        flash_.sleep();
    }
    else{
        Serial.println("SPI Flash MEM not found (is chip soldered?)...");
    }
}

void DeviceCore::initRadio(uint8_t nodeId)
{
    const auto &config = radioConfig_.data();
    if(radio_.initialize(RF69_868MHZ, nodeId, config.networkId))
    {
        Serial.println(F("init radio: "));
        Serial.print(F("nodeId=["));Serial.print(nodeId, DEC);Serial.println(F("]"));
        Serial.print(F("networkId=["));Serial.print(config.networkId, DEC);Serial.println(F("]"));

        if(config.customFrequency){
            Serial.print(F("customFrequency=["));Serial.print(config.customFrequency, DEC);Serial.println(F("]"));
            radio_.setFrequency(config.customFrequency);
        }

        if(strlen(config.encryptKey)){
            Serial.print(F("encryptKey=["));Serial.print(config.encryptKey);Serial.println(F("]"));
            radio_.encrypt(config.encryptKey);
        }

        radio_.setHighPower(true);
        radio_.setPowerLevel(config.powerLevel);
        
        radio_.sleep();
        Serial.println(F("radio started in sleep mode!"));
    }
    else{
        Serial.println(F("radio initialize failed!"));
        while(1){}
    }
}

// Messages
bool DeviceCore::registerNode()
{
    if(nodeRegistered_){
        return true;    // Nothing to do here
    }

    PRINTLND1F("registerNode start");
    // Reset radio ID to default one
    initRadio(radioConfig_.getUnregisteredNodeId());

    uint8_t attempts = 1;
    
    NodeRegisterFrame nodeRegisterFrame;
    while(!nodeRegistered_){
        // Every 10th attempt sleep for 1 minute, for 30 seconds try to send message every 3 seconds
        long nDelay = attempts++ % 3 == 0 ? 60000U : 5000U;

        PRINTLND1F("loop");
        NodeRegisterRequest nodeRegisterRequest;
        strncpy(nodeRegisterRequest.uuid, uuidConfig_.data().uuid, sizeof(nodeRegisterRequest.uuid));
        nodeRegisterRequest.deviceClass = deviceClass_;
        nodeRegisterRequest.sleepTime = informNodeAlivePeriodSec_  + 1;

        nodeRegisterFrame.build(buffer_, nodeRegisterRequest);

        if(!sendBufferToRadio(true)){
            deepSleepDelay(nDelay);
            continue;
        }

        // Check if received response
        if(buffer_.size())
        {
            if(!verifyBaseFrame(ActionType::Register, ActionDirection::Response)){
                deepSleepDelay(nDelay);
                continue;
            }

            NodeRegisterResponse nodeRegisterResponse;
            if(!nodeRegisterFrame.parse(nodeRegisterResponse, buffer_)){
                PRINTLND1F("NodeRegisterResponse parse err");
                deepSleepDelay(nDelay);
                continue;
            }

            if(nodeRegisterResponse.error != ActionError::Ok){
                PRINTLND1F("register error!"); 
                deepSleepDelay(nDelay);
                continue;
            }

            PRINTD1F("new nodeId:"); PRINTLND1(nodeRegisterResponse.nodeId);
            initRadio(nodeRegisterResponse.nodeId);   // Initialize radio with ID from id's pull
            nodeRegistered_ = true;
        }
        else{
            deepSleepDelay(nDelay);
        }
    }

    PRINTD1F("node registered!");

    return true;
}

bool DeviceCore::informNodeAlive()
{
    batteryState_.read();
    NodeAliveRequest request;
    request.batteryPercent = batteryState_.computePercent();

    NodeAliveFrame nodeAliveFrame;
    if(!nodeAliveFrame.build(buffer_, request)){
        PRINTLND1F("NodeAliveRequest build err");
        return false;
    }

    if(!sendBufferToRadio(true)){
        return false;
    }

    // Check if received response
    if(buffer_.size())
    {
        if(!verifyBaseFrame(ActionType::Alive, ActionDirection::Response)){
            return false;
        }

        NodeAliveResponse response;
        if(!nodeAliveFrame.parse(response, buffer_)){
            PRINTLND1F("NodeAliveResponse parse err");
            return false;
        }

        if(response.error != ActionError::Ok){
            PRINTLND1F("not registered!");
            nodeRegistered_ = false; 
            return false;
        }
    }

    return true;
}

bool DeviceCore::getCommand()
{
    return false;
}


bool DeviceCore::sendBufferToRadio(bool ack)
{
    PRINTD1F("-->radio(ack:");PRINTD1(ack);PRINTD1F("): ");PRINTLND1(buffer_.data());

    if(ack)
    {
        bool rc = radio_.sendWithRetry(radioConfig_.data().gatewayId, buffer_.data(), strlen(buffer_.data()));
        if(ackCheck(rc))
        {
            PRINTLND1F("radio sent!");
            if(buffer_.size())
            {
                PRINTD1F("<--radio: "); PRINTLND1(buffer_.data());
            }
            return true;
        }
        else
        {
            PRINTLND1F("send failed!");
            return false;
        }
    }
    else
    {
        radio_.send(radioConfig_.data().gatewayId, buffer_.data(), strlen(buffer_.data()));
        return true;
    }
}

bool DeviceCore::loadBufferFromRadio()
{
    if(radio_.receiveDone()){
        const char *data = reinterpret_cast<const char*>(radio_.DATA);
        PRINTD1F("<--radio: "); PRINTLND1(data);
        return loadRadioDataToBuffer();
    }
    return false;
}

bool DeviceCore::sendBufferToSerial()
{
    Serial.println(buffer_.data());
    return true;
}

bool DeviceCore::loadBufferFromSerial()
{
    if(Serial.available())
    {
        buffer_.clear();
        if(Serial.readBytesUntil(serialTerminator_, buffer_.data(), sizeof(buffer_)))
        {
            PRINTD1F("-->serial: "); PRINTLND1(buffer_.data());
            return true;
        }
    }

    return false;
}

bool DeviceCore::ackCheck(bool rc)
{
    PRINTLND1F("ackCheck");
    buffer_.clear();
    if (rc) 
    {
        noInterrupts();
        loadRadioDataToBuffer();
        interrupts();
    }
    return rc;
}

bool DeviceCore::loadRadioDataToBuffer()
{
    PRINTLND1F("lrdtb");
    const char *data = reinterpret_cast<const char*>(radio_.DATA);
    // tranmission ok, see if any data was Ack'd
    if (radio_.DATALEN) 
    {
        PRINTLND1F("DATALEN");
        if(radio_.DATALEN == strlen(data))  // got a valid packet?
        {
            PRINTLND1F("if cp");
            strncpy(buffer_.data(), data, buffer_.Size);
            return true;
        }  
        return false;
    }
    else
    {
        return true;
    }
}

void DeviceCore::setInformNodeAlivePeriodSec(uint16_t period)
{
    informNodeAlivePeriodSec_ = period;
}

uint16_t DeviceCore::getInformNodeAlivePeriodSec()
{
    return informNodeAlivePeriodSec_;
}

// void DeviceCore::initRadio()
// {
//     const auto &config = radioConfig_.data();
//     if(radio_.initialize(RF69_868MHZ, config.networkId, config.networkId))
//     {
//         Serial.println(F("init radio: "));
//         Serial.print(F("nodeId=["));Serial.print(config.networkId, DEC);Serial.println(F("]"));
//         Serial.print(F("networkId=["));Serial.print(config.networkId, DEC);Serial.println(F("]"));

//         if(config.customFrequency){
//             Serial.print(F("customFrequency=["));Serial.print(config.customFrequency, DEC);Serial.println(F("]"));
//             radio_.setFrequency(config.customFrequency);
//         }

//         if(config.isHCW){
//             Serial.print(F("isHCW=["));Serial.print(config.isHCW, DEC);Serial.println(F("]"));
//             radio_.setHighPower();  //must include this only for RFM69HW/HCW!
//         }

//         if(strlen(config.encryptKey)){
//             Serial.print(F("isHCW=["));Serial.print(config.isHCW, DEC);Serial.println(F("]"));
//             radio_.encrypt(config.encryptKey);
//         }

//         radio_.setPowerLevel(config.powerLevel);
        
//         radio_.sleep();
//         Serial.println(F("radio started in sleep mode!"));
//     }
//     else{
//         Serial.println(F("radio initialize failed!"));
//         while(1){}
//     }
// }

// bool DeviceCore::sendBufferToRadio()
// {
//     serializeJson(jsonDoc_, buffer_);

//     PRINTD1F("AirOut: "); PRINTLND1(buffer_);

//     if(radio_.sendWithRetry(radioConfig_.data().gatewayId, buffer_, strlen(buffer_)))
//     {
//         PRINTLND1F("radio sent!");
//         if (radio_.ACKRequested())
//         {
//             radio_.sendACK();
//             PRINTLND1F("ACK sent!");
//             return true;
//         }
//         else
//         {
//             return false;
//         }
//     }
//     else
//     {
//         PRINTLND1F("send failed!");
//         return false;
//     }
// }

// bool DeviceCore::loadBufferFromRadio()
// {
//     if(radio_.receiveDone()){
//         const char *data = reinterpret_cast<const char*>(radio_.DATA);
//         PRINTD1F("RadioIn: "); PRINTLND1(data);
//         deserializeJson(jsonDoc_, data);
//         return true;
//     }
//     return false;
// }   

// bool DeviceCore::sendBufferToSerial()
// {
//     serializeJson(jsonDoc_, Serial);
//     return true;
// }

// bool DeviceCore::loadBufferFromSerial(char terminator)
// {
//     if(Serial.readBytesUntil(terminator, buffer_, sizeof(buffer_)))
//     {
//         PRINTD1F("SerialIn: "); PRINTLND1(buffer_);
//         DeserializationError error = deserializeJson(jsonDoc_, buffer_);
//         if (error) {
//             Serial.print(F("deserializeJson() failed: "));
//             Serial.println(error.f_str());
//             return false;
//         }
//         return true;
//     }
//     return false;
// }

// void DeviceCore::waitForConfig(unsigned int timeAmount)
// {
//     radioConfig_.read();
//     bool isConfigDone = radioConfig_.data().version;
//     Serial.println("wait for cfg");
//     while(1)
//     {
//         if(loadBufferFromSerial() /*|| loadBufferFromRadio()*/){
//             const char *operation = jsonDoc_[F("cmd")];
//             if(!strcmp_P(operation, PSTR("grc"))){
//                 JsonObject jsonData = jsonDoc_.createNestedObject(F("data"));
//                 RFRadioConfigJSON cfg;
//                 if(cfg.build(radioConfig_.data(), jsonData)){
//                     sendBufferToSerial();
//                 }
//             }
//             else{
//                 PRINTLND1F("op not supported!");
//             }
//         }
//     }
// }

// void DeviceCore::clearBuffer()
// {
//     memset(buffer_, '\0', sizeof(buffer_));
// }

// void DeviceCore::clearJsonDoc()
// {
//     jsonDoc_.clear();
// }

void DeviceCore::deepSleepDelay(unsigned int delay)
{
    Serial.flush();
    radio_.sleep();

    LowPowerWrp.DeepSleep(delay);
}

void wakeUp()
{
    DeviceCore::IsExtInterrupt = true;
}

void DeviceCore::deepSleepForewerAndWakeInt(uint8_t pin_, uint8_t mode)
{
    Serial.flush();
    radio_.sleep();
    attachInterrupt(digitalPinToInterrupt(pin_), wakeUp, mode);
    IsExtInterrupt = false;    // Clear interrupt flag
    LowPowerWrp.DeepSleepForever();
    detachInterrupt(0); 
}

bool DeviceCore::verifyBaseFrame(ActionType actionType, ActionDirection actionDirection)
{
    BaseFrameData baseFrameData;
    BaseFrame baseFrame;
    if(!baseFrame.parse(buffer_, baseFrameData)){
        PRINTLND1F("BaseFrameData parse err");
        return false;
    }

    if(baseFrameData.actionType != actionType || baseFrameData.actionDirection != actionDirection){
        PRINTLND1F("BaseFrameData not passed!");
        return false;
    }

    return true;
}


// void DeviceCore::appendSendBufferFloat(double value, uint8_t prec)
// {
//     char temp[15];
//     dtostrf(value, 4, 1, temp);
//     strcat(sendBuffer_, temp);
// }

#pragma once

static const char *ActionDelim = "|";

enum class ActionType{
    // Node Actions
    Register = 1,
    Alive = 2,
    State = 3,

    // System Actions
    NodeIdentifierConfig = 100,
    RadioConfig = 101,
    DeviceConfig = 102

};

enum class ActionDirection{
    Request = 1,
    Response = 2
};
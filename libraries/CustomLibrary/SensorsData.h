#pragma once
#include <ArduinoJson.h>

struct SensorsData
{
    SensorsData();
    String serialize() const;

    int pulse_;
    float roomTemperature_;
    bool isFlooded_;
    bool isInProximity_;
    bool isGasPresent_;
    bool isLightPresent_;
};

enum SensorPin
{
    PULSE_SENSOR = 0,
    GAS_SENSOR = 3,
    LIGHT_SENSOR = 6,
    PROXIMITY_SENSOR = 7
};
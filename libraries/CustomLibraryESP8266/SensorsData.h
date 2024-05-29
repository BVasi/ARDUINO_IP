#pragma once
#include <ArduinoJson.h>

struct SensorsData
{
    SensorsData();
    String serialize(const String& cnpOfPerson) const;

    int pulse_;
    float roomTemperature_;
    bool isFlooded_;
    bool isInProximity_;
    bool isGasPresent_;
    bool isLightPresent_;
};

enum SensorPin
{
    PULSE_SENSOR = 36,
    GAS_SENSOR = 18,
    LIGHT_SENSOR = 1,
    PROXIMITY_SENSOR = 3,
    ROOM_TEMPERATURE_SENSOR_SDA = 21,
    ROOM_TEMPERATURE_SENSOR_SCL = 22
};
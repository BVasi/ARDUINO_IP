#pragma once
#include <ArduinoJson.h>

enum AlarmType
{
    NONE,
    GAS_DETECTED,
    IS_FLOODED,
    TEMPERATURE_INCREASE,
    UNUSUAL_BODY_PARAMETERS,
    SYSTEM_IS_DOWN
};

class Alarm
{
    public:
        Alarm(const String& aditionalText, const AlarmType& alarmType);
        String serialize(const String& cnpOfPerson) const;
    private:
        String aditionalText_;
        AlarmType alarmType_;
};
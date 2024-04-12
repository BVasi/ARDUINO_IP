#include "Alarm.h"

const String ALARM = "alarm";
const String ADITIONAL_TEXT = "additionalText";
const String ALARM_TYPE = "alarmType";
const String CNP = "cnp";

Alarm::Alarm(const String& aditionalText, const AlarmType& alarmType)
{
    aditionalText_ = aditionalText;
    alarmType_ = alarmType;
}

String Alarm::serialize(const String& cnpOfPerson) const
{
    JsonDocument restData;
    JsonObject alarmData = restData.createNestedObject(ALARM);
    String serializedData;
    alarmData[ADITIONAL_TEXT] = aditionalText_;
    alarmData[ALARM_TYPE] = alarmType_;
    alarmData[CNP] = cnpOfPerson;
    serializeJson(restData, serializedData);
    return serializedData;
}
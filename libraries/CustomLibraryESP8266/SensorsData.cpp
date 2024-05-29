#include <SensorsData.h>

const int DEFAULT_VALUE_FOR_INT = -1;
const float DEFAULT_VALUE_FOR_FLOAT = -1.0f;
const bool DEFAULT_VALUE_FOR_BOOL = false;
const String SENSORS_DATA = "sensorsData";
const String PULSE = "pulse";
const String IS_GAS = "is_gas_present";
const String IS_LIGHT = "light";
const String IS_IN_PROXIMITY = "is_in_proximity";
const String ROOM_TEMPERATURE = "room_temperature";
const String IS_FLOODED = "humidity";
const String CNP = "cnp";

SensorsData::SensorsData()
{
    pulse_ = DEFAULT_VALUE_FOR_INT;
    roomTemperature_ = DEFAULT_VALUE_FOR_FLOAT;
    isFlooded_ = DEFAULT_VALUE_FOR_BOOL;
    isInProximity_ = DEFAULT_VALUE_FOR_BOOL;
    isGasPresent_ = DEFAULT_VALUE_FOR_BOOL;
    isLightPresent_ = DEFAULT_VALUE_FOR_BOOL;
}

String SensorsData::serialize(const String& cnpOfPerson) const
{
    JsonDocument restData;
    JsonObject sensorsData = restData.createNestedObject(SENSORS_DATA);
    String serializedData;
    sensorsData[PULSE] = pulse_;
    sensorsData[IS_GAS] = isGasPresent_;
    sensorsData[IS_LIGHT] = isLightPresent_;
    sensorsData[IS_IN_PROXIMITY] = isInProximity_;
    sensorsData[ROOM_TEMPERATURE] = roomTemperature_;
    sensorsData[IS_FLOODED] = isFlooded_;
    restData[CNP] = cnpOfPerson;
    serializeJson(restData, serializedData);
    return serializedData;
}
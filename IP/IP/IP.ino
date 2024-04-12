#include <PulseSensorPlayground.h>
#include <Ticker.h>
#include "Adafruit_Si7021.h"
#include "SensorsData.h"
#include "SensorsSettings.h"
#include "NetworkWrapper.h"
#include "HttpWrapper.h"

namespace EndPoints
{
  const String REMOTE_SERVER_ADDRESS = "http://127.0.0.1:5000";
  const String SAVE_SENSORS_DATA = REMOTE_SERVER_ADDRESS + "/save_sensors_data";
  const String CHANGE_SENSORS_SETTINGS = "/change_sensors_settings";
  const String REPORT_ALARM = REMOTE_SERVER_ADDRESS + "/report_alarm";
};

const int ONE_SECOND = 1000;
const int PULSE_THRESHOLD = 300;
const int SERVER_PORT = 80;

SensorsSettings sensorsSettings = SensorsSettings();
Adafruit_Si7021 roomTemperatureSensor = Adafruit_Si7021();
NetworkWrapper networkWrapper = NetworkWrapper();
HttpWrapper httpWrapper = HttpWrapper(SERVER_PORT);
PulseSensorPlayground pulseSensor;
Ticker sendDataTimer;
String cnp = "-1";

void handleChangeSensorSettings()
{
  JsonDocument receivedData;
  DeserializationError error = deserializeJson(receivedData, httpWrapper.getPlainData());
  if (error)
  {
    httpWrapper.sendResponse(400, "");
    return;
  }

  String newSamplingPeriod = receivedData["newSensorSettings"]["samplingPeriod"];
  //mapeaza la enum-ul din sensorsSettings
  //ia datele din referinte, baga-le in sensorsSettings
  httpWrapper.sendResponse(200, "");
}

SensorsData getSensorsData()
{
  SensorsData sensorsData = SensorsData();
  int rawValue = analogRead(SensorPin::LIGHT_SENSOR);
  float voltage = rawValue * (3.3 / 1023) * 1000;
  float resitance = 10000 * (voltage / (5000.0 - voltage));
  if (pulseSensor.sawStartOfBeat())
  {
    sensorsData.pulse_ = pulseSensor.getBeatsPerMinute();
  }
  sensorsData.roomTemperature_ = roomTemperatureSensor.readTemperature();
  sensorsData.isFlooded_ = (65 < roomTemperatureSensor.readHumidity());
  sensorsData.isInProximity_ = (0 < digitalRead(SensorPin::PROXIMITY_SENSOR));
  sensorsData.isGasPresent_ = (0 < digitalRead(SensorPin::GAS_SENSOR));
  sensorsData.isLightPresent_ = (resitance < 7000);

  return sensorsData;
}

void setupPulseSensor()
{
  pulseSensor.analogInput(SensorPin::PULSE_SENSOR);
  pulseSensor.setThreshold(PULSE_THRESHOLD);
  if (!pulseSensor.begin())
  {
    Serial.println("Did not find pulse sensor!");
  }
}

void setupRoomTemperatureSensor()
{
  if (!roomTemperatureSensor.begin())
  {
    Serial.println("Did not find SI7021 sensor!");
  }
}

AlarmType shouldRaiseAlarm(const SensorsData& sensorsData)
{
  if (sensorsData.isFlooded_)
  {
    return AlarmType::IS_FLOODED;
  }
  if (sensorsData.isGasPresent_)
  {
    return AlarmType::GAS_DETECTED;
  }
  if (sensorsSettings.sensorsReferences_.shouldRaiseAlarm(sensorsData) != AlarmType::NONE)
  {
    return sensorsSettings.sensorsReferences_.shouldRaiseAlarm(sensorsData);
  }
  return AlarmType::NONE;
}

String getAdditionalAlarmText(const SensorsData& sensorsData, const AlarmType& alarmType)
{
  String additionalAlarmText = "";
  switch (alarmType)
  {
    case GAS_DETECTED:
    {
      additionalAlarmText = "Gaz detectat!";
      break;
    }
    case IS_FLOODED:
    {
      additionalAlarmText = "Inundatie detectata!";
      break;
    }
    case TEMPERATURE_INCREASE:
    {
      additionalAlarmText = "Temperatura a depasit " + String(sensorsData.roomTemperature_) + " grade Celsius!";
      break;
    }
    case UNUSUAL_BODY_PARAMETERS:
    {
      additionalAlarmText = "Pulsul a fost " + String(sensorsData.pulse_) + " BPM!";
      break;
    }
  }
  return additionalAlarmText;
}

void sendPostRequestWrapper()
{
  SensorsData sensorsData = getSensorsData();
  AlarmType alarmType = AlarmType::NONE;
  if (((alarmType = shouldRaiseAlarm(sensorsData)) != AlarmType::NONE))
  {
    Alarm alarm = Alarm(getAdditionalAlarmText(sensorsData, alarmType), alarmType);
    httpWrapper.sendPostRequest(EndPoints::REPORT_ALARM, alarm.serialize(cnp));
  }
  httpWrapper.sendPostRequest(EndPoints::SAVE_SENSORS_DATA, sensorsData.serialize());
}

void setup()
{
  Serial.begin(9600);
  Serial.print("Introduceti SSID: ");
  String ssid = Serial.readStringUntil('\n');
  Serial.print("Introduceti parola: ");
  String password = Serial.readStringUntil('\n');
  Serial.print("Introduceti CNP-ul: ");
  cnp = Serial.readStringUntil('\n');
  networkWrapper.setNetworkDetails(ssid.c_str(), password.c_str());
  networkWrapper.connectToNetwork();
  setupPulseSensor();
  setupRoomTemperatureSensor();
  pinMode(SensorPin::PROXIMITY_SENSOR, INPUT);
  httpWrapper.on(EndPoints::CHANGE_SENSORS_SETTINGS + "/" + cnp, handleChangeSensorSettings);
  sendDataTimer.attach(sensorsSettings.samplingPeriod_, sendPostRequestWrapper);
}

void loop()
{
  if (!networkWrapper.isConnectedToNetwork())
  {
    networkWrapper.connectToNetwork();
  }
  httpWrapper.handleClient();
  delay(ONE_SECOND);
}
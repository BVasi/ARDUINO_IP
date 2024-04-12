#include <NetworkWrapper.h>

NetworkWrapper::NetworkWrapper()
{
    ssid_ = "";
    password_ = "";
}

void NetworkWrapper::setNetworkDetails(const char* ssid, const char* password)
{
    ssid_ = ssid;
    password_ = password;
}

void NetworkWrapper::connectToNetwork() const
{
    WiFi.begin(ssid_, password_);
    while (!isConnectedToNetwork())
    {
        Serial.println("Trying to connect to the network...");
        delay(MILISECONDS_BETWEEN_RETRIES);
    }
    Serial.println("Connection established!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

bool NetworkWrapper::isConnectedToNetwork() const
{
    return (WiFi.status() == WL_CONNECTED);
}
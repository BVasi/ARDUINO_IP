#pragma once
#include <ESP8266WiFi.h>

const int MILISECONDS_BETWEEN_RETRIES = 1000;

class NetworkWrapper
{
    public:
        NetworkWrapper();

        void setNetworkDetails(const char* ssid, const char* password);
        void connectToNetwork() const;
        bool isConnectedToNetwork() const;
    private:
        String ssid_;
        String password_;
};
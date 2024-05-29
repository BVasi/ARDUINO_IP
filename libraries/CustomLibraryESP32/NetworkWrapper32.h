#pragma once
#include <WiFi.h>

const int MILISECONDS_BETWEEN_RETRIES = 1000;

class NetworkWrapper
{
    public:
        NetworkWrapper();
        NetworkWrapper(const NetworkWrapper&) = delete;
        NetworkWrapper& operator=(const NetworkWrapper&) = delete;

        void setNetworkDetails(const char* ssid, const char* password);
        void connectToNetwork() const;
        bool isConnectedToNetwork() const;
    private:
        const char* ssid_;
        const char* password_;
};
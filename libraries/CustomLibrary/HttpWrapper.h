#pragma once
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>

enum HttpResponses
{
    HTTP_OK = 200,
    HTTP_NOT_FOUND = 404
};

class HttpWrapper
{
    public:
        HttpWrapper(const int SERVER_PORT);

        void handleClient();
        void sendPostRequest(const String& destination, const String& request);
        void on(const String& route, ESP8266WebServer::THandlerFunction handler);
        void sendResponse(const int& statusCode, const String& content);
        String getPlainData();
    private:
        ESP8266WebServer server_;
};
#include "HttpWrapper.h"

const char* CONTENT_TYPE = "Content-Type";
const char* JSON_DATA = "application/json";
const char* TEXT_PLAIN = "text/plain";
const char* PLAIN = "plain";

HttpWrapper::HttpWrapper(const int SERVER_PORT) : server_(SERVER_PORT)
{
    server_.begin();
}

void HttpWrapper::handleClient()
{
    server_.handleClient();
}

void HttpWrapper::on(const String& route, ESP8266WebServer::THandlerFunction handler)
{
    server_.on(route, handler);
}

void HttpWrapper::sendPostRequest(const String& destination, const String& request)
{
    WiFiClient wifiClient;
    HTTPClient httpClient;
    httpClient.begin(wifiClient, destination);
    httpClient.addHeader(CONTENT_TYPE, JSON_DATA);
    int responseCode = httpClient.POST(request);

    if (responseCode == HTTP_OK)
    {
        Serial.println("Send data ok!");
    }
    else if (responseCode == HTTP_NOT_FOUND)
    {
        Serial.println("Send data failed!");
    }
    httpClient.end();
}

void HttpWrapper::sendResponse(const int& statusCode, const String& content)
{
    server_.send(statusCode, TEXT_PLAIN, content);
}

String HttpWrapper::getPlainData()
{
    return server_.arg(PLAIN);
}
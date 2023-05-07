#ifndef _ESP8266FSAUTOCONNECT_
#define _ESP8266FSAUTOCONNECT_
#include <ESP8266WiFi.h>       // Include the ESP8266WiFi library
#include <ESPAsyncWebServer.h> // Include the ESPAsyncWebServer library
#include <ESPAsyncTCP.h>       // Include the ESPAsyncTCP library
#include "LittleFS.h"

#define CREDS_PATH "/wificreds.bin"

class ESP8266FSAutoConnect {
private:
    AsyncWebServer *_ssta_server = NULL;
    AsyncWebServer * _main_server = NULL;
    uint16_t _port = 80;
    bool _connect_sta = false;
    bool _sta_connected = false;
    unsigned long _timer = 0;
    struct _creds
    {
        String ap_ssid = "ESP8266FSAutoConnect"; // defaults in case any issue
        String ap_pass = "12345678";             // defaults in case any issue
        String sta_ssid = "";
        String sta_pass = "";
    } wc;
    uint8_t _conn_count = 0;
    WiFiEventHandler staDisconnectedHandler, staGotIPHandler;
    bool initCreds(void);
    bool saveCreds(void);
    bool validateCreds(String ssid, String pass);
    bool getWiFiCreds(uint8_t * data, String * ssid, String * pass);
    void handleCredsBody(AsyncWebServerRequest * req, uint8_t *data, size_t len, size_t index, size_t total);
    void startAPServer(void);
    void displayWC(void);

    void connectToAP(void);
public:
    ESP8266FSAutoConnect(uint16_t port);
    bool autoConnect(AsyncWebServer * server);
    void run(void);
};

#endif _ESP8266FSAUTOCONNECT_
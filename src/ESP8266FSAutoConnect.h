#ifndef _ESP8266FSAUTOCONNECT_
#define _ESP8266FSAUTOCONNECT_
#include <ESP8266WiFi.h>       // Include the ESP8266WiFi library
#include <ESPAsyncWebServer.h> // Include the ESPAsyncWebServer library
#include <ESPAsyncTCP.h>       // Include the ESPAsyncTCP library
#include "LittleFS.h"
#include "index.h"
#include "styles.h"
#include "script.h"
#include "WiFiSecrets.h"

#define CREDS_PATH "/wificreds.bin"


#ifdef DEBUG
#define _PP(...) USE_SERIAL.print(__VA_ARGS__)
#define _PL(...) USE_SERIAL.println(__VA_ARGS__)
#define _PF(...) USE_SERIAL.printf(__VA_ARGS__)
#else
#define _PP(...)
#define _PL(...)
#define _PF(...)
#endif

struct _creds {
    String ap_ssid;
    String ap_pass;
    String sta_ssid;
    String sta_pass;
};

class ESP8266FSAutoConnect {
private:
    // ------ Private Variables ------
    AsyncWebServer *_ssta_server = NULL;
    AsyncWebServer * _main_server = NULL;
    Stream *_libSerial = NULL;
    uint16_t _port = 80;
    bool _connect_sta = false;
    bool _sta_connected = false;
    bool _connect_AP = false;
    unsigned long _timer = 0;
    _creds wc;
    _creds wc_default {APSSID, APPASS, STASSID, STAPASS}; // default values from WiFiSecrets.h file
    uint8_t _conn_count = 0;
    bool _ap_server_running = false;
    bool _main_server_running = false;
    WiFiEventHandler staDisconnectedHandler, staGotIPHandler;

    // ------ Private Functions ------
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
    void setSerial(Stream *serialReference);
};

#endif //_ESP8266FSAUTOCONNECT_
#include "ESP8266FSAutoConnect.h"

ESP8266FSAutoConnect::ESP8266FSAutoConnect(uint16_t port)
{
    _port = port;
}
bool ESP8266FSAutoConnect::initCreds(void)
{
    if (!LittleFS.begin())
    {
        Serial.println("LittleFS initialization unsuccessful");
        Serial.println("Resorting to default AP creds");
        return false;
    }
    //LittleFS.format(); //for testing only
    if (!LittleFS.exists(CREDS_PATH))
    {
        Serial.printf("No file found at %s\n", CREDS_PATH);
        Serial.println("Resorting to default AP credentials");
        return false;
    }

    File credsFile = LittleFS.open(CREDS_PATH, "r");
    if (!credsFile)
    {
        Serial.printf("Failed to read credentials file at %s\n", CREDS_PATH);
        Serial.println("Resorting to default AP creds");
        return false;
    }
    wc.ap_ssid = credsFile.readStringUntil('\n');
    wc.ap_pass = credsFile.readStringUntil('\n');
    wc.sta_ssid = credsFile.readStringUntil('\n');
    wc.sta_pass = credsFile.readStringUntil('\n');
    credsFile.close();

    if(!validateCreds(wc.sta_ssid, wc.sta_pass)){
        Serial.println("bad STA credentials, Resorting to AP creds");
        return false;
    }
    return true;
}

bool ESP8266FSAutoConnect::saveCreds(void){
    if (!LittleFS.begin())
    {
        Serial.println("LittleFS check unsuccessful");
        Serial.println("Try restarting the device");
        return false;
    }
    File credsFile = LittleFS.open(CREDS_PATH, "w");
    if(!credsFile){
        Serial.printf("Failed to create file at %s\n", CREDS_PATH);
        return false;
    }
    credsFile.printf("%s\n%s\n%s\n%s\n",wc.ap_ssid.c_str(), wc.ap_pass.c_str(), wc.sta_ssid.c_str(), wc.sta_pass.c_str());
    credsFile.close();
    return true;
}

bool ESP8266FSAutoConnect::getWiFiCreds(uint8_t *data, String *ssid, String *pass)
{
    String body = (char *)data;
    int nsi = body.indexOf("\"ssid\":\"") + 8;
    if (nsi == -1)
    {
        return false;
    }
    int nei = body.indexOf("\",", nsi);
    if (nei == -1)
    {
        return false;
    }
    *ssid = body.substring(nsi, nei);
    nsi = body.indexOf("\"pass\":\"") + 8;
    if (nsi == -1)
    {
        return false;
    }
    nei = body.indexOf("\"}", nsi);
    if (nei == -1)
    {
        return false;
    }
    *pass = body.substring(nsi, nei);
    return true;
}

bool ESP8266FSAutoConnect::validateCreds(String ssid, String pass)
{
    if (ssid.length() == 0 || pass.length() == 0)
    {
        return false;
    }
    if (ssid.length() > 32)
    {
        return false;
    }
    if (pass.length() < 8 || pass.length() > 63)
    {
        return false;
    }
    return true;
}

void ESP8266FSAutoConnect::displayWC(void){
    Serial.printf("ap_ssid : %s\nap_pass : %s\nsta_ssid : %s\nsta_pass : %s\n", wc.ap_ssid.c_str(), wc.ap_pass.c_str(), wc.sta_ssid.c_str(), wc.sta_pass.c_str());
}

void ESP8266FSAutoConnect::startAPServer(void){
    Serial.println("Starting AP");
    WiFi.disconnect();
    WiFi.mode(WIFI_AP_STA);
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    WiFi.softAP(wc.ap_ssid, wc.ap_pass);
    _connect_AP = true;
}

void ESP8266FSAutoConnect::connectToAP(void){
    WiFi.softAPdisconnect();
    WiFi.disconnect();
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(wc.sta_ssid, wc.sta_pass);
}

bool ESP8266FSAutoConnect::autoConnect(AsyncWebServer *server)
{
    
    _main_server = server;
    
    staGotIPHandler = WiFi.onStationModeGotIP([&](const WiFiEventStationModeGotIP& event){
        Serial.println("Connected to Wi-Fi sucessfully.");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        if (_ap_server_running){
            _ssta_server->end();
            _ap_server_running = false;
            Serial.println("AP Server END");
        }
        if(this->_main_server){
            if(!this->_main_server_running){
                this->_main_server->begin();
                this->_main_server_running = true;
                Serial.println("starting main server");
            }
        }
        this->_conn_count = 0;
        this->_sta_connected = true;
    });
    staDisconnectedHandler = WiFi.onStationModeDisconnected([this](const WiFiEventStationModeDisconnected& event){
        if(this->_conn_count < 5){
            Serial.printf("Disconnected from Wi-Fi Attempting reconnect : %d\n", this->_conn_count);
            this->connectToAP();
            this->_conn_count++;
            return;
        }
        
        this->startAPServer();

    });

        _ssta_server = new AsyncWebServer(_port);
        _ssta_server->on("/", HTTP_GET, [](AsyncWebServerRequest *req){ 
            req->send_P(200, "text/html", _index); 
        });
        _ssta_server->on("/styles.css", HTTP_GET, [](AsyncWebServerRequest *req){
            req->send_P(200, "text/css", _styles);
        });
        _ssta_server->on("/script.js", HTTP_GET, [](AsyncWebServerRequest *req){
            req->send_P(200, "application/javascript", _script);
        });
        _ssta_server->onRequestBody([this](AsyncWebServerRequest *req, uint8_t *data, size_t len, size_t index, size_t total){
             if (len > 150)
            {
                req->send(400, "text/plain", "body too large");
                return;
            }
            if (req->url() != "/update")
            {
                req->send(400, "text/plain", "bad request");
                return;
            }
            if (!req->hasParam("type"))
            {
                req->send(400, "text/plain", "wrong parameter name");
                return;
            }
            String type = req->getParam("type")->value();

            String ssid = "";
            String pass = "";
            if (!this->getWiFiCreds(data, &ssid, &pass))
            {
                req->send(400, "text/plain", "error parsing json body");
                return;
            }
            if(!this->validateCreds(ssid, pass)){
                req->send(400, "text/plain", "bad ssid/password length");
                return;
            }
            if(type == "AP"){
                this->wc.ap_ssid = ssid;
                this->wc.ap_pass = pass;
                if(!this->saveCreds()){
                    req->send(500, "text/plain", "Failed to save AP credentials");
                    return;
                }
                req->send(200, "text/plain", "AP updated, in effect after device restart");
                return;
            }
            if(type == "STA"){
                this->wc.sta_ssid = ssid;
                this->wc.sta_pass = pass;
                if(!this->saveCreds()){
                    req->send(500, "text/plain", "Failed to save STA credentials");
                    return;
                }
                req->send(200, "text/plain", "STA updated attemping to connect to AP...");
                
                this->_conn_count = 0;
                this->_connect_sta = true;
                this->_sta_connected = false;
                return;
            }
            req->send(400, "text/plain", "invalid parameter");
        });

    if (!initCreds())
    {
        
        
        startAPServer();
        
        return false;
    }
    Serial.printf("Connecting to access point : %s\n", wc.sta_ssid);
    connectToAP();
    return true;
}

void ESP8266FSAutoConnect::run(){
    if(millis() - _timer > 100){
        if(_connect_sta == true && _sta_connected == false){
                _sta_connected = true;
                connectToAP();
        }
        if (_connect_AP)
        {
                _connect_AP = false;
                if (_main_server)
                {
                    if (_main_server_running)
                    {
                        _main_server->end();
                        _main_server_running = false;
                        Serial.println("Main server END");
                    }
                }
                if (!_ap_server_running)
                {
                    _ssta_server->begin();
                    _ap_server_running = true;
                    Serial.printf("Started AP Server on port %d\n", _port);
                }
        }
        _timer = millis();
    }
    
}
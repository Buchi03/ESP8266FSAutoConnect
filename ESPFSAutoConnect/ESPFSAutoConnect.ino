#include "ESP8266FSAutoConnect.h"
#include <ESPAsyncWebServer.h>
ESP8266FSAutoConnect EFAC(80);
AsyncWebServer server(80);
void setup(){
  Serial.begin(115200);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * req){
    req->send(200, "text/plain", "hello from main server");
  });
  
  EFAC.autoConnect(&server);
//  WiFi.begin(STA_SSID, STA_PASS);
//  while(WiFi.status() != WL_CONNECTED){
//    Serial.println("."); 
//  }
//  Serial.println(WiFi.localIP());
//  server.begin();
}

void loop(){
  EFAC.run();
}

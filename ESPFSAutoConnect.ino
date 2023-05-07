#include "ESP8266FSAutoConnect.h"

ESP8266FSAutoConnect EFAC(80);
void setup(){
  Serial.begin(115200);
  EFAC.autoConnect(NULL);
}

void loop(){
  
}

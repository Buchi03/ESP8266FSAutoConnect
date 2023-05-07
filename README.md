# ESP8266FSAutoConnect

### A project attempting to create a stable WiFi manager, mainly to achieve a quick WiFi credential switch without any code changes and without/minor restarts.

### Features  
* Asynchronous - Mainly based on ESP8266 WiFi events to connect to WiFi instead of the regular blocking methods that generally take a longer time for connection.
* Reliable - On of the goals of this project is to minimize the ESP8266 restarts so as to avoid any delays in say : data logging.
* AutoReconnect - No need to hardcode router credentials in the code, just need to give the credentials once and you are good to go.  
The credentials are preserved even after code reupload as they are stored in the File system of ESP8266. (Note : This feature works only if the "Erase Flash" option  
of build options is set to "Only Sketch")
* Maintains Connection : In case the ESP8266 disconnects from the access point (router) then the code automatically reattemps to connect 3 times (by default) to the  
access point, in case all 3 attemps are unsuccessful, the ESP8266 launches a softAP server as usual to update the credentials.

### Dependencies  
* [LittleFS](https://github.com/littlefs-project/littlefs)
* [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
* [ESPAsyncTCP](https://github.com/me-no-dev/ESPAsyncTCP)

### Detailed Block Diagram

<img src="https://raw.githubusercontent.com/Prateek7805/ESP8266FSAutoConnect/master/Block%20Diagram.png"/>

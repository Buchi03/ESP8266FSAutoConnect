#include <ESP8266WiFi.h>       // Include the ESP8266WiFi library
#include <ESPAsyncTCP.h>       // Include the ESPAsyncTCP library
#include "LittleFS.h"
#include "index.h"
#include "styles.h"
#include "script.h"

#define CREDS_PATH "/wificreds.bin"

// Debugging Flag
#define DEBUG

// Define serial debugging interface
#define USE_SERIAL Serial

#ifdef DEBUG
#define _PP(...) USE_SERIAL.print(__VA_ARGS__)
#define _PL(...) USE_SERIAL.println(__VA_ARGS__)
#define _PF(...) USE_SERIAL.printf(__VA_ARGS__)
#else
#define _PP(...)
#define _PL(...)
#define _PF(...)
#endif
#include <Arduino.h>
// Standard ESP8266 WiFi library
#include <ESP8266WiFi.h>
// Multicast DNS library to allow "<name>.local" access
#include <ESP8266mDNS.h>
// Username and password for wifi
#include "auth.h"

void setup_wifi(const char *name, const char *ssid, const char *password);
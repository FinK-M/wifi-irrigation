#include "wifi.h"

void setup_wifi(const char *name, const char *ssid, const char *password){
  // Start the access point
  WiFi.softAP(ssid, password);             
  Serial.printf("Access Point \"%s\" started\r\n", ssid);

  // Send the IP address of the ESP8266 to the computer
  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP());         

  // Start the mDNS responder for esp8266.local
  if (!MDNS.begin(name)){ Serial.println("Error setting up MDNS responder!"); }
  else{ Serial.println("mDNS responder started"); }
}
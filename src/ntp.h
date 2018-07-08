#include <Arduino.h>
// Standard ESP8266 WiFi library
#include <ESP8266WiFi.h>
// NTP uses UDP, not TCP
#include <WiFiUdp.h>
// For daylight saving time
#include <Timezone.h>
// Standard time functions and types
#include <TimeLib.h>

#define CALC_ACTUAL_TIME (time_unix + (millis_current - last_ntp_response)/1000)
#define TIME_TO_REFRESH (millis_current - last_ntp_request > NTP_refresh_time)
#define UPDATE_TIMEOUT ((millis_current - last_ntp_response) > 3600000)
#define WLAN_DISCONNECTED (WiFi.status() != WL_CONNECTED)

// NTP time stamp is in the first 48 bytes of the message
const int NTP_PACKET_SIZE = 48; 
// Refresh rate in milliseconds
const unsigned long NTP_refresh_time = 60000;

void setup_NTP();
bool NTP_loop();
String time_string(time_t time);

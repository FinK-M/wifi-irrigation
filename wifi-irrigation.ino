#include "src/header.h"

// Number of solenoids connected to MCU
extern const int NUM_SOLENOIDS = 4;
// Pin on which solenoid is located
extern const int solenoid_pins[NUM_SOLENOIDS] = {D5, D6, D7, D8};
// Solenoid states (True is disabled as pullup)
bool solenoid_states[NUM_SOLENOIDS] = {true, true, true, true};
// time variable
extern time_t time_utc;
extern time_t time_local;
// The name of the Wi-Fi network that will be created
const char *ssid = "ESP8266 Access Point";
// The password required to connect to it, leave blank for an open network
const char *password = "password";
// Give a domain name for mDNS
const char *name = "test-portal";

// Create a webserver object that listens for HTTP request on port 80
ESP8266WebServer server(80);
// Create a websocket server on port 81
WebSocketsServer webSocket = WebSocketsServer(81);


void setup() {
    // Set solenoid pins as output and drive them high
    for(uint8_t i = 0; i < NUM_SOLENOIDS; i++){
      pinMode(solenoid_pins[i], OUTPUT);
      digitalWrite(solenoid_pins[i], HIGH);
    } 

    // Clear any previously stored WiFi configuration
    ESP.eraseConfig();

    Serial.begin(115200);
    delay(10);
    Serial.println('\n');

    setup_fs();
    setup_wifi(name, ssid, password);
    setup_server();
    setup_socket();
    setup_NTP();
    //setup_OTA("ESP8266", "esp8266");
}

void loop(void){
    // Check for new sketch to upload
    ArduinoOTA.handle();
    // Check for socket events
    webSocket.loop();                   
    // Listen for HTTP requests from clients
    server.handleClient();
    // Check for NTP update events
    if(NTP_loop()){
        int i = 0;
        while(webSocket.sendTXT(i++, String("TIME:UTC:") + time_string(time_utc)));
        i = 0;
        while(webSocket.sendTXT(i++, String("TIME:LOCAL:") + time_string(time_local)));
    }
}



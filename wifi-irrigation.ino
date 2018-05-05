#include "header.h"

// Pin on which solenoid is located
const int sol_pin = D8;
// The name of the Wi-Fi network that will be created
const char *ssid = "ESP8266 Access Point";
// The password required to connect to it, leave blank for an open network
const char *password = "password";
// Give a domain name for mDNS
const char *name = "solenoid";

// Create a webserver object that listens for HTTP request on port 80
ESP8266WebServer server(80);
// Create a websocket server on port 81
WebSocketsServer webSocket = WebSocketsServer(81);

extern bool sol_state;


void setup() {
    // Set solenoid pin as output and drive it high
    pinMode(sol_pin, OUTPUT);
    digitalWrite(sol_pin, HIGH);  

    // Clear any previously stored WiFi configuration
    ESP.eraseConfig();

    Serial.begin(115200);
    delay(10);
    Serial.println('\n');

    setup_fs();
    setup_wifi(name, ssid, password);
    setup_server();
    setup_socket();   
}

void loop(void){
    // Check for socket events
    webSocket.loop();                   
    // Listen for HTTP requests from clients
    server.handleClient();
    digitalWrite(D8, sol_state);
}



#include "src/header.h"

// Vector of valve object
vector<Valve> valves{D5, D6, D7, D8};
// time without respect to time zone or daylight savings
extern time_t time_utc;
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
        while(webSocket.sendTXT(i++, String("TIME:LOCAL:") + time_string(now())));
        valve_checker();
    }
}

void valve_checker(void){
    for (auto& v : valves){
        if(v.update_state()){
            int i = 0;
            while(webSocket.sendTXT(
                i++, 
                String("SOL:") + v.code + ":" + v.get_state()));
        }
    }
}



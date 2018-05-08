#include "wifi.h"

const int retries = 10;

const char *SSID = "22 Camside 2.4GHz";
const char *PASSWORD = "StrongAndStable";

// Static function prototypes
static bool setup_client(const char *name, const char *ssid);
static void setup_ap(const char *ssid, const char *password);
static void setup_mdns(const char *name);


void setup_wifi(const char *name, const char *ssid, const char *password){
	// Try to connect to a network, otherwise create AP
	if(!setup_client(SSID, PASSWORD)){ setup_ap(ssid, password); }
	// Setup domain name
	setup_mdns(name);
}


static bool setup_client(const char *ssid, const char *password){
  	// Initialise connection process
  	WiFi.begin(ssid, password);             
  	Serial.printf("Connecting to %s ...", ssid);

  	int i = 0;
  	// Wait for the Wi-Fi to connect, or timeout
  	while (WiFi.status() != WL_CONNECTED) { 
    	delay(1000);
    	Serial.printf("%d ", ++i);
    	// Connection timed out
    	if (retries == i){ 
            // Disable station mode
            WiFi.disconnect(true);
            Serial.println("Failed to connect");
            return false;
        }
  	}
  	Serial.printf(
  		"\n\nConnection established!\r\n"
  		"IP address:\t"
  	);
	Serial.println(WiFi.localIP());

	// Succesfully connected
	return true;
}


static void setup_ap(const char *ssid, const char *password){
	// Start the access point
	WiFi.softAP(ssid, password);             
	Serial.printf("Access Point \"%s\" started\r\n", ssid);

	// Send the IP address of the ESP8266 to the computer
	Serial.print("IP address:\t");
	Serial.println(WiFi.softAPIP());         
}


static void setup_mdns(const char *name){
	// Start the mDNS responder for esp8266.local
	if (!MDNS.begin(name)){ Serial.println("Error setting up MDNS responder!"); }
	else{ Serial.println("mDNS responder started"); }
}
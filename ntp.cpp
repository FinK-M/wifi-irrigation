#include "ntp.h"

// Create an instance of the WiFiUDP class to send and receive
WiFiUDP UDP;
// time.nist.gov NTP server address
IPAddress timeServerIP;
// Address of time server
const char* NTPServerName = "pool.ntp.org";
// buffer to hold incoming and outgoing packets
byte NTPBuffer[NTP_PACKET_SIZE];

// Data recieved from getTime() function - may be invalid
static uint32_t time_ntp = 0;
// As above, but checked for valididty
static uint32_t time_unix = 0;

// Time converted to instantaneous second count
uint32_t time_seconds = 0;
// As above, but previous reading
static uint32_t time_seconds_prev = 0;

// Time since last ntp request was sent
static uint32_t last_ntp_request = 0;
// Time since last ntp response was actually recieved
static uint32_t last_ntp_response = millis();
// Current time in ms
static uint32_t millis_current = millis();

static uint32_t getTime();
static void sendNTPpacket(IPAddress& address);

inline int getSeconds(uint32_t UNIXTime) { return UNIXTime % 60; }
inline int getMinutes(uint32_t UNIXTime) { return UNIXTime / 60 % 60; }
inline int getHours(uint32_t UNIXTime) { return UNIXTime / 3600 % 24; }

void setup_NTP(){
	UDP.begin(123);
	// Get the IP address of the NTP server
	if(!WiFi.hostByName(NTPServerName, timeServerIP)) {
    	Serial.println("DNS lookup failed. Rebooting.");
    	Serial.flush();
    	ESP.reset();
  	}
  	Serial.print("Time server IP:\t");
  	Serial.println(timeServerIP);
  
  	Serial.println("\r\nSending NTP request ...");
  	sendNTPpacket(timeServerIP); 
}

String format_time(uint32_t time){
	return String("") + \
		getHours(time) + ":" + getMinutes(time) + ":" + getSeconds(time);
}


uint32_t NTP_loop() {
  	millis_current = millis();
 	// If a minute has passed since last NTP request
  	if (TIME_TO_REFRESH) {
    	last_ntp_request = millis_current;
    	Serial.println("\r\nSending NTP request ...");
    	// Send an NTP request
    	sendNTPpacket(timeServerIP);               
  	}

  	// Check if an NTP response has arrived and get the (UNIX) time
  	time_ntp = getTime();
  	 // If a new timestamp has been received
  	if (time_ntp) {                                 
    	time_unix = time_ntp;
    	Serial.print("NTP response:\t");
    	Serial.println(time_unix);
    	last_ntp_response = millis_current;
  	}
  	else if (UPDATE_TIMEOUT) {
    	Serial.println("More than 1 hour since last NTP response. Rebooting.");
    	Serial.flush();
    	ESP.reset();
  	}
  	time_seconds = CALC_ACTUAL_TIME;
	// If a second has passed since last print
	if (time_seconds != time_seconds_prev && time_unix != 0) { 
	    time_seconds_prev = time_seconds;
	    return true;
	}
	else { return false; }
}

static uint32_t getTime() {
	// If there's no response (yet)
  	if (UDP.parsePacket() == 0) { return 0; }
  	// read the packet into the buffer
	UDP.read(NTPBuffer, NTP_PACKET_SIZE);
	// Combine the 4 timestamp bytes into one 32-bit number
	uint32_t NTPTime = (NTPBuffer[40] << 24) | (NTPBuffer[41] << 16) | (NTPBuffer[42] << 8) | NTPBuffer[43];
	// Convert NTP time to a UNIX timestamp:
	// Unix time starts on Jan 1 1970. That's 2208988800 seconds in NTP time:
	const uint32_t seventyYears = 2208988800UL;
	// subtract seventy years:
	uint32_t UNIXTime = NTPTime - seventyYears;
	return UNIXTime;
}

static void sendNTPpacket(IPAddress& address) {
	// set all bytes in the buffer to 0	
  	memset(NTPBuffer, 0, NTP_PACKET_SIZE);  
  	// Initialize values needed to form NTP request
  	// LI, Version, Mode
  	NTPBuffer[0] = 0b11100011;   
  	// send a packet requesting a timestamp:
  	// NTP requests are to port 123
  	UDP.beginPacket(address, 123);
  	UDP.write(NTPBuffer, NTP_PACKET_SIZE);
  	UDP.endPacket();
}
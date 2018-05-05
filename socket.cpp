#include "socket.h"

extern WebSocketsServer webSocket;

bool sol_state = false;

static void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght);

// Start a WebSocket server
void setup_socket() {
  // start the websocket server
  webSocket.begin();                          
  // if there's an incomming websocket message, go to function 'webSocketEvent'
  webSocket.onEvent(webSocketEvent);
  Serial.println("WebSocket server started.");
}

static void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {
    switch (type) {

        // if the websocket is disconnected
        case WStype_DISCONNECTED:             
            Serial.printf("[%u] Disconnected!\n", num);
            break;

        // if a new websocket connection is established
        case WStype_CONNECTED: {          
            IPAddress ip = webSocket.remoteIP(num);
            Serial.printf(
                "[%u] Connected from %d.%d.%d.%d url: %s\n",
                num, ip[0], ip[1], ip[2], ip[3], payload);
            }             
            break;
      

        // if new text data is received
        case WStype_TEXT:{                    
            Serial.printf("[%u] get Text: %s\n", num, payload);
            String text = String((char*)payload);
            if (text.startsWith("SOL")) {
              switch (text[4]) {
                  case '1':
                    Serial.println("Solenoid 1");
                    break;
                  case '2':
                    Serial.println("Solenoid 2");
                    break;
                  case '3':
                    Serial.println("Solenoid 3");
                    break;
                  case '4':
                    Serial.println("Solenoid 4");
                    break;
                  default:
                    Serial.println("Default");
              }
                
                sol_state = !sol_state;
                webSocket.sendTXT(0, "Solenoid Switched");
            }
            }
            break;

        default:
            Serial.printf("Unhandled Signal Type\n");
    }
}

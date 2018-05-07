#include "socket.h"

extern WebSocketsServer webSocket;

extern bool solenoid_states[];
extern int solenoid_pins[];
extern int NUM_SOLENOIDS;

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
              Serial.printf("Solenoid %c switched\r\n", text[4]);
              char buffer[20];
              
              int sol_num = text[4] - '0' - 1;
              solenoid_states[sol_num] = !solenoid_states[sol_num];
              digitalWrite(solenoid_pins[sol_num], solenoid_states[sol_num]);
              sprintf(buffer, "SOL:%c:%d", text[4], !solenoid_states[sol_num]);
              webSocket.sendTXT(num, buffer);
                
            }
            else if (text.startsWith("?")){
              if (text.startsWith("STATE", 1)){
                Serial.println("Sending Solenoid State");
                char buffer[20];
                for(int i = 0; i < NUM_SOLENOIDS; i++){
                  sprintf(buffer, "SOL:%d:%d", i + 1, !solenoid_states[i]);
                  webSocket.sendTXT(num, buffer);
                }
              }
            }
            }
            break;

        default:
            Serial.printf("Unhandled Signal Type\n");
    }
}

//static void command_interpreter(String command, uint8_t num)

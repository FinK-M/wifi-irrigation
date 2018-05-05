#include "socket.h"

extern WebSocketsServer webSocket;

extern bool sol_state_1;
extern bool sol_state_2;
extern bool sol_state_3;
extern bool sol_state_4;
extern int solenoid_1;
extern int solenoid_2;
extern int solenoid_3;
extern int solenoid_4;

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
              
              
              switch (text[4]) {
                  case '1':
                    sol_state_1 = !sol_state_1;
                    digitalWrite(solenoid_1, sol_state_1);
                    sprintf(buffer, "SOL:%c:%d", text[4], !sol_state_1);
                    break;
                  case '2':
                    sol_state_2 = !sol_state_2;
                    digitalWrite(solenoid_2, sol_state_2);
                    sprintf(buffer, "SOL:%c:%d", text[4], !sol_state_2);
                    break;
                  case '3':
                    sol_state_3 = !sol_state_3;
                    digitalWrite(solenoid_3, sol_state_3);
                    sprintf(buffer, "SOL:%c:%d", text[4], !sol_state_3);
                    break;
                  case '4':
                    sol_state_4 = !sol_state_4;
                    digitalWrite(solenoid_4, sol_state_4);
                    sprintf(buffer, "SOL:%c:%d", text[4], !sol_state_4);
                    break;
                  default:
                    Serial.println("Default");
              }
              webSocket.sendTXT(num, buffer);
                
            }
            }
            break;

        default:
            Serial.printf("Unhandled Signal Type\n");
    }
}

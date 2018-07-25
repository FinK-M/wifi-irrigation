#include "socket.h"
// Server instance created in main sketch
extern WebSocketsServer webSocket;
// Vector containing solenoid valve objects
extern vector<Valve> valves;

static void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght);
static void command_interpreter(String command, uint8_t num);

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
            // Convert payload from uint8_t array to string object
            String text = String((char*)payload);
            command_interpreter(text, num);
            }
            break;

        default:
            Serial.printf("Unhandled Signal Type\n");
    }
}

static void command_interpreter(String command, uint8_t num){
  // Buffer for socket messages                    
  char buffer[20];

  if (command.startsWith("SOL")) {
    Serial.printf("Solenoid %c switched\r\n", command[4]);
    // Convert ASCII character to equivalent int, then offset by 1
    int sol_num = command[4] - '0' - 1;
    // Flip chosen solenoid state and change flag
    valves[sol_num].toggle_state();
    // Construct confirmation message and send into socket
    sprintf(buffer, "SOL:%c:%d", command[4], valves[sol_num].get_state());
    webSocket.sendTXT(num, buffer);
      
  }
  else if (command.startsWith("TIME")){
    int h = command.substring(5, 7).toInt();
    int m = command.substring(8, 10).toInt();
    Serial.println(h);
    Serial.println(m);
  }
  else if(command.startsWith("RUN")){
    int r = command.substring(4, command.length()).toInt();
    Serial.println(r);
  }
  // '?' is query character
  else if (command.startsWith("?")){
    // Get status of all solenoids
    if (command.startsWith("STATE", 1)){
      Serial.println("Sending Solenoid State");
      // Send individual messages for each solenoid to reuse .js code for
      // setting individual button colours
      for (auto v : valves){
        sprintf(buffer, "SOL:%d:%d", v.code, v.get_state());
        webSocket.sendTXT(num, buffer);
      }
    }
  }
}

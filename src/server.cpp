#include "server.h"

extern ESP8266WebServer server;

// function prototypes for HTTP handlers
static void handleRoot();              
static void handleNotFound();
static bool handleFileRead(String path);
static String getContentType(String filename);

static void handleRoot() {
  // Send HTTP status 200 (Ok) and send some text to the browser/client
  server.send(200, "text/plain", "Hello world!");
}

static void handleNotFound(){
  // check if the file exists in the flash memory (SPIFFS), if so, send it
  if(!handleFileRead(server.uri())){          
    server.send(404, "text/plain", "404: File Not Found");
  }
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";          // If a folder is requested, send the index file
  String contentType = getContentType(path);             // Get the MIME type
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) { // If the file exists, either as a compressed archive, or normal
    if (SPIFFS.exists(pathWithGz))                         // If there's a compressed version available
      path += ".gz";                                         // Use the compressed verion
    File file = SPIFFS.open(path, "r");                    // Open the file
    size_t sent = server.streamFile(file, contentType);    // Send it to the client
    file.close();                                          // Close the file again
    Serial.println(String("\tSent file: ") + path);
    return true;
  }
  Serial.println(String("\tFile Not Found: ") + path);   // If the file doesn't exist, return false
  return false;
}

String getContentType(String filename) { // determine the filetype of a given filename, based on the extension
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

void setup_server(){
  // When a client requests an unknown URI (i.e. something other than "/"),
  // call function "handleNotFound"              
  server.onNotFound(handleNotFound);        
  // Actually start the server
  server.begin();
}
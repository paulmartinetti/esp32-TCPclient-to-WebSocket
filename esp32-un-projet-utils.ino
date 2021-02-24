/**
 * Adapted from Arduino > Examples > WiFi > SimpleWiFiServer
 * 
 * The esp32 is a TCP client that gets data from a WebSocket on a Node Express server
 * 
 */

#include <WiFi.h>
#define ARDUINOJSON_ENABLE_ARDUINO_STREAM 1
#include <ArduinoJson.h>
#include <StreamUtils.h>

// WiFi
const char* ssid = "";
const char* password = "";

// Web socket IP address
const char* host = "";

void setup()
{
  Serial.begin(115200);
  delay(10);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // connect to WiFi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// restrict looping if unnecessary
int loopCount = 0;

// Highly recommend using Json Assistant
// https://arduinojson.org/v6/assistant/
StaticJsonDocument<384> doc;
DeserializationError error;

void loop() {

  // I don't see a difference without the delay..it's only faster
  //delay(5000);
  ++loopCount;
  // loop only once
  if (loopCount > 1) return;

  Serial.print("connecting to ");
  Serial.println(host);

  // Create TCP client
  WiFiClient client;
  // port is on the node express server (see server code in README.md)
  const int httpPort = 1337;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // Handshake offer from client
  client.print(String("GET ") + "HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  // stream to json logger is nice for debugging via Serial Monitor
  while (client.available()) {
    ReadLoggingStream loggingClient(client, Serial);
    error = deserializeJson(doc, loggingClient);
  }
  if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }

  //fetch json values, for example, depending on your json
  int id = doc["id"]; // 5
  Serial.print("id: "); Serial.println(id);
}

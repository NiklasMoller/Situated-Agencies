#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

#include "Secrets.h"

char ssid[] = mySSID;
char pass[] = myPASSWORD;

const char* mqtt_server = "broker.mqttdashboard.com"; 

WiFiClient client;
PubSubClient pubSubClient(client);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup() {

  Serial.begin(9600);

  setupWifi();

  pubSubClient.setServer(mqtt_server, 1883);
  //pubSubClient.setCallback(callback);

}

void loop() {

  float sensorValue = analogRead(A0);   // read the input on analog pin 0

  //float voltage = sensorValue * (5.0 / 1023.0);   // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V)

  //460
  int newValue = map(sensorValue, 460, 1023, 0, 100);
  //Serial.println(newValue);   // print out the value you read

  if (!pubSubClient.connected()) {
    reconnect();
  }

  pubSubClient.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, "hello world Niklas #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    pubSubClient.publish("situated-agencies", msg);
  }

}


void setupWifi(){

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.println("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi Connected");
  printWiFiStatus();
  
}

// print Wifi status
void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void reconnect() {
  // Loop until we're reconnected
  while (!pubSubClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (pubSubClient.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      pubSubClient.publish("situated-agencies", "hello world reconnect");
      // ... and resubscribe
      //pubSubClient.subscribe("situated-agencies");
    } else {
      Serial.print("failed, rc=");
      Serial.print(pubSubClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

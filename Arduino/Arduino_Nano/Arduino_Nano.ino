#include <SPI.h>
#include <WiFiNINA.h>
#include <MQTT.h>
#include <avr/dtostrf.h>

//Add seperate file for sensitive info in the future
//#include "arduino_secrets.h"
char ssid[] = "SKYD60F5";        // your network SSID (name)
char pass[] = "LTFWNLQCLD";    // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;
MQTTClient MClient;
char pubCharsPayload[100];

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();

  //Connecting to MQTT
  MClient.begin("broker.shiftr.io", client);
//  MClient.onMessage(messageReceived);
  connect();
}

void connect(){
  //Connecting to MQTT
  Serial.print("\nconnecting to mqtt...");
  //Name of person (can be anything), user key, user password(both provided by shiftr)
  while (!MClient.connect("Arduino Nano","763f6861","1362154b898a179a")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected to mqtt!");
  MClient.subscribe("/test");
}

void messageReceived(String &topic, String &payload) {
  if (topic == "/test"){
  Serial.println("Test Message Receieved!");
}
}

void loop() {

 MClient.loop();

  if (!MClient.connected()) {
    connect();
  }

      //Construct the payload as a JSON object
    String payload = "Arduino Says Hello!";
    payload.toCharArray(pubCharsPayload, (payload.length() + 1));
    delay(5000);
    MClient.publish("/test", pubCharsPayload);
    
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

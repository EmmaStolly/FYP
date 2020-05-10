#include <SPI.h>
#include <WiFiNINA.h>
#include <MQTT.h>
#include <avr/dtostrf.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

//Add seperate file for sensitive info in the future
//#include "arduino_secrets.h"
char ssid[] = "SKYD60F5";        // your network SSID (name)
char pass[] = "LTFWNLQCLD";    // your network password

int status = WL_IDLE_STATUS;

//TFT Pins
#define TFT_DC 9              
#define TFT_CS 10             
#define TFT_RST 8             
#define TFT_MISO 12           
#define TFT_MOSI 11           
#define TFT_CLK 13   

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

WiFiClient client;
MQTTClient MClient;

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

  tft.begin();
   tft.setRotation(1);
  tft.fillScreen(ILI9341_BLUE);
  tft.setTextSize(8);
  tft.setTextColor(ILI9341_RED); 
//  tft.println("Hello");
//  tft.println("World!");
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
  MClient.onMessage(messageReceived);
}

void messageReceived(String &topic, String &payload) {
  if (topic == "/test"){
    yield();
      tft.println(payload);
  }
}

void loop() {
  MClient.loop();
   if (!client.connected()) {
    connect();
  }
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

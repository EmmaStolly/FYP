//Libraries ------------------------------------------
#include <SPI.h>
#include <WiFiNINA.h>
#include <TinyGPS.h>
#include <MQTT.h>
#include <avr/dtostrf.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

//Add seperate file for sensitive info in the future - Network credentials
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
TinyGPS gps;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
//  Initialise RX TX Serial
  Serial1.begin(9600);

//  WiFi Connection ------------------------------------------
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
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();

// MQTT Connection ------------------------------------------
  MClient.begin("broker.shiftr.io", client);
  connect();

//TFT Setup ------------------------------------------
  tft.begin();
//  Set to landscape
   tft.setRotation(1);
  tft.setTextSize(8);
  tft.setTextColor(ILI9341_RED);
  tft.fillScreen(ILI9341_WHITE);
  
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
//  Subscribe to MQTT topic
    MClient.subscribe("messages");
//  Trigggered when message received from subscribed topic
  MClient.onMessage(messageReceived);
 }

void messageReceived(String &topic, String &payload) {
  if (topic == "messages"){

//    Sending message to TFT LCD ------------------------------------------
//    Clear previous message
    tft.fillScreen(ILI9341_WHITE);
//    Reset to line 1 of TFT
    tft.setCursor(0, 0);
//    Print message to TFT
      tft.println(payload);
  }
}

void loop() {
//  MQTT Connection Loop ------------------------------------------
//  Remain connected to MQTT
  MClient.loop();
   if (!client.connected()) {
    connect();
  }


//Send GPS data to web app ------------------------------------------
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

    // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial1.available())
    {
      char c = Serial1.read();
      // Serial.write(c); // uncomment for raw GPS data
//      Looking for incoming data
      if (gps.encode(c))
        newData = true;
    }
  }

  if (newData)
  {
    float flat, flon;
    unsigned long age;
    
    gps.f_get_position(&flat, &flon, &age);
//    Convert GPS data into string
    String latitude = String(flat,6); 
    String longitude = String(flon,6); 
//Serial.println(latitude+";"+longitude); //Uncomment to see GPS data in serial monitor
//Convert location data into a JSON format
String locationPayload = "{\"Latitude\":" + latitude + ",\"Longitude\":" + longitude + "}";
//Serial.println(locationPayload); //Uncomment for GPS JSON in serial monitor

//Push GPS every 3 seconds (Can be changed)
delay(10000);
//Publish to "/topic", GPS data
MClient.publish("/location", locationPayload);
}

}

//Wifi Connection Info ------------------------------------------
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

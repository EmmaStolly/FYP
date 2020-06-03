//Libraries ------------------------------------------
#include <SPI.h>
#include <WiFiNINA.h>
#include <TinyGPS.h>
#include <MQTT.h>
#include <avr/dtostrf.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

//Fill with your wifi name and password
char ssid[] = "";        // your network SSID (name)
char pass[] = "";    // your network password

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

  //TFT Setup ------------------------------------------
  tft.begin();
//  Set to landscape
   tft.setRotation(1);
  tft.setTextSize(8);
  tft.setTextColor(ILI9341_BLACK);
  tft.fillScreen(ILI9341_WHITE);
  tft.println("Hello!");
  

// MQTT Connection ------------------------------------------
  MClient.begin("broker.shiftr.io", client);
  connect();
}

void connect(){
  //Connecting to MQTT
  Serial.print("\nconnecting to mqtt...");
  //Name of person (can be anything), mqtt user key, mqtt user password(both provided by shiftr)
  while (!MClient.connect("Arduino Nano","","")) { //Replace with your details
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected to mqtt!");
//  Subscribe to MQTT topics
    MClient.subscribe("messages");
    MClient.subscribe("custom/font/red");
    MClient.subscribe("custom/font/yellow");
    MClient.subscribe("custom/font/orange");
    MClient.subscribe("custom/font/green");
    MClient.subscribe("custom/font/blue");
    MClient.subscribe("custom/font/pink");
    MClient.subscribe("custom/font/purple");
    MClient.subscribe("custom/font/black");
    MClient.subscribe("custom/font/white");
    MClient.subscribe("custom/background/red");
    MClient.subscribe("custom/background/orange");
    MClient.subscribe("custom/background/yellow");
    MClient.subscribe("custom/background/green");
    MClient.subscribe("custom/background/blue");
    MClient.subscribe("custom/background/pink");
    MClient.subscribe("custom/background/purple");
    MClient.subscribe("custom/background/black");
    MClient.subscribe("custom/background/white");
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
      Serial.println(payload);
  }

//  Sending custom coloured fonts to TFT LCD  ------------------------------------------
    if (topic == "custom/font/red"){
      Serial.println("Font set to red");
    tft.setTextColor(ILI9341_RED);
  }
      if (topic == "custom/font/orange"){
      Serial.println("Font set to orange");
    tft.setTextColor(ILI9341_ORANGE);
  }
      if (topic == "custom/font/yellow"){
      Serial.println("Font set to yellow");
    tft.setTextColor(ILI9341_YELLOW);
  }
     if (topic == "custom/font/green"){
      Serial.println("Font set to green");
    tft.setTextColor(ILI9341_GREEN);
  }
      if (topic == "custom/font/blue"){
      Serial.println("Font set to blue");
    tft.setTextColor(ILI9341_BLUE);
  }
      if (topic == "custom/font/pink"){
      Serial.println("Font set to pink");
    tft.setTextColor(ILI9341_PINK);
  }
      if (topic == "custom/font/purple"){
      Serial.println("Font set to purple");
    tft.setTextColor(ILI9341_PURPLE);
  }
        if (topic == "custom/font/black"){
      Serial.println("Font set to black");
    tft.setTextColor(ILI9341_BLACK);
  }
        if (topic == "custom/font/white"){
      Serial.println("Font set to white");
    tft.setTextColor(ILI9341_WHITE);
  }

//Sending custom coloured backgrounds to TFT LCD  ------------------------------------------
      if (topic == "custom/background/red"){
      Serial.println("background set to red");
    tft.fillScreen(ILI9341_RED);
  }
      if (topic == "custom/background/orange"){
      Serial.println("background set to orange");
    tft.fillScreen(ILI9341_ORANGE);
  }
      if (topic == "custom/background/yellow"){
      Serial.println("background set to yellow");
    tft.fillScreen(ILI9341_YELLOW);
  }
     if (topic == "custom/background/green"){
      Serial.println("background set to green");
    tft.fillScreen(ILI9341_GREEN);
  }
      if (topic == "custom/background/blue"){
      Serial.println("background set to blue");
    tft.fillScreen(ILI9341_BLUE);
  }
      if (topic == "custom/background/pink"){
      Serial.println("background set to pink");
    tft.fillScreen(ILI9341_PINK);
  }
      if (topic == "custom/background/purple"){
      Serial.println("background set to purple");
    tft.fillScreen(ILI9341_PURPLE);
  }
        if (topic == "custom/background/black"){
      Serial.println("background set to black");
    tft.fillScreen(ILI9341_BLACK);
  }
        if (topic == "custom/background/white"){
      Serial.println("background set to white");
    tft.fillScreen(ILI9341_WHITE);
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

//If there is new location data
  if (newData)
  {
//    variables
    float flat, flon;
    unsigned long age;
//    Get GPS Data
    gps.f_get_position(&flat, &flon, &age);
//    Convert GPS data into string
    String latitude = String(flat,6); 
    String longitude = String(flon,6); 
//Serial.println(latitude+";"+longitude); //Uncomment to see GPS data in serial monitor
//Convert location data into a JSON format
String locationPayload = "{\"Latitude\":" + latitude + ",\"Longitude\":" + longitude + "}";
//Serial.println(locationPayload); //Uncomment for GPS JSON in serial monitor

//Push GPS every 10 seconds (Can be changed)
delay(10000);
//Publish to "/location", GPS data
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

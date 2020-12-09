// ------------------- blog.make2explore.com---------------------------------------------------------------//
// Project           - ESP32 LoRa Weather Station + Web Server
// Created By        - blog@make2explore.com
// Version - 1.0
// Software Platform - C/C++,HTML,CSS, Arduino IDE, Libraries
// Hardware Platform - TTGo LoRa ESP32 OLED v1, 0.96" OLED, Arduino MKRWAN 1300, BMP280 Sensor, DHT22 Sensor                
// Sensors Used      - BMP280 Atm Pressure Sensor, DHT22 Temperature Humidity Sensor
// Last Modified     - 31/03/2020 21:44:00
// -------------------------------------------------------------------------------------------------------//
// This is Receiver Node Arduino MKR1300 WAN Code - 

//Libraries for LoRa WAN
#include <SPI.h>
#include <LoRa.h>

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 433E6


//OLED pins
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels


// Initialize variables to get and save LoRa data
int rssi;
String loRaMessage;
String temperature;
String humidity;
String pressure;
String readingID;

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


//------------------Initialize OLED Module-------------------------------------------//
void initOLED(){
  //initialize OLED
  Serial.println("Initializing OLED Module..");
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
 }
  Serial.println("OLED Initialization OK!");
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(20,0);
  display.print("LORA RECEIVER NODE");
  display.display(); 
}

//----------------------Initialize LoRa Module-----------------------------------------//
void initLoRA(){
  Serial.println("Initializing LoRa Module..");
  int counter;
  while (!LoRa.begin(BAND) && counter < 10) {
    Serial.print(".");
    counter++;
    delay(500);
  }
  if (counter == 10) {
    // Increment readingID on every new reading
    Serial.println("Starting LoRa failed!"); 
  }
  Serial.println("LoRa Initialization OK!");
  display.setCursor(0,20);
  display.clearDisplay();
  display.print("LoRa Initializing OK!");
  display.display();
  delay(2000);
}

//----------------------Initialize all modules-----------------------------------------//
void initialize_modules()
{
  Serial.println("Initializing Required Modules");
  initOLED();
  initLoRA();
  Serial.println("Initialization Done!!");
  delay(1000);
}

//-----------------Read LoRa packet and get the sensor readings-----------------------//
void getLoRaData() {
  Serial.print("Lora packet received: ");
  // Read packet
  while (LoRa.available()) {
    String LoRaData = LoRa.readString();
    // LoRaData format: readingID/temperature&soilMoisture#batterylevel
    // String example: 1/27.43&654#95.34
    Serial.println(LoRaData); 
    
    // Get readingID, temperature and soil moisture
    int pos1 = LoRaData.indexOf('/');
    int pos2 = LoRaData.indexOf('&');
    int pos3 = LoRaData.indexOf('#');
    readingID = LoRaData.substring(0, pos1);
    temperature = LoRaData.substring(pos1 +1, pos2);
    humidity = LoRaData.substring(pos2+1, pos3);
    pressure = LoRaData.substring(pos3+1, LoRaData.length());    
  }
  // Get RSSI
  rssi = LoRa.packetRssi();
  Serial.print(" with RSSI ");    
  Serial.println(rssi);
}

//-------------------------Display Readings on OLED-------------------------------------//
void displayReadings()
{
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("LoRa RECEIVER 2 -Pune");
  display.setCursor(0,20);
  display.setTextSize(1);
  display.print("TEMPERATURE");
  display.setCursor(70,20);
  display.print(":");
  display.setCursor(80,20);
  display.print(temperature);
  display.drawCircle(116,20,1, WHITE);
  display.setCursor(121,20);
  display.print("C");
  display.setCursor(0,37);
  display.print("HUMIDITY(%)");
  display.setCursor(70,37);
  display.print(":");
  display.setCursor(80,37);
  display.print(humidity);
  display.setCursor(116,37);
  display.print("Rh");
  display.setCursor(0,55);
  display.print("PRESSURE");
  display.setCursor(50,55);
  display.print(":");
  display.setCursor(60,55);
  display.print(pressure);
  display.setCursor(100,55);
  display.print("mmHg");
  display.display();
  delay(1000);
}

void setup() {
  Serial.begin(115200);
  Serial.println("LoRa Receiver Node");
  initialize_modules();
  Serial.println("Ready To Receive Data");
}

void loop() {
  // Check if there are LoRa packets available
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    getLoRaData();
    displayReadings();
    delay(1000);
   }
}

//============================ www.blog.make2explore.com ==============================//

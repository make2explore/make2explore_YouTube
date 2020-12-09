// ------------------- blog.make2explore.com---------------------------------------------------------------//
// Project           - ESP32 LoRa Weather Station + Web Server
// Created By        - blog@make2explore.com
// Version - 1.1
// Software Platform - C/C++,HTML,CSS, Arduino IDE, Libraries
// Hardware Platform - TTGo LoRa ESP32 OLED v1, 0.96" OLED, Arduino MKRWAN 1300, BMP280 Sensor, DHT22 Sensor                
// Sensors Used      - BMP280 Atmospheric Pressure Sensor, DHT22 Temperature & Humidity Sensor
// Last Modified     - 31/03/2020 21:44:00
// -------------------------------------------------------------------------------------------------------//
// This is Sender Node Code - 

//Libraries for LoRa
#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>

//Libraries for BMP280
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

//Libraries for OLED Display
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Libraries for DHT22/11
#include <DHT.h>
#include <DHT_U.h>

//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 433E6

//OLED pins
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Digital pin connected to the DHT sensor
#define DHTPIN 25
#define DHTTYPE DHT22  // Type DHT 22 (AM2302)

//global variables for temperature and Humidity
float Temperature = 0;
float Humidity = 0;
float Pressure = 0;

//initilize packet counter
int readingID = 0;
String LoRaMessage = "";

//define BMP instance
Adafruit_BMP280 bme; // I2C

////define OLED instance
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

//define DHT instance
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;


//------------------Initialize OLED Module-------------------------------------------//
void initOLED()
{
  Serial.println("Initializing OLED Display");
  //reset OLED display via software
  
  //initialize OLED
  Wire.begin();

  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  Serial.println("OLED Display OK!");
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("LoRa SENDER 1 - Pune");
  display.display();
}

//----------------------Initialize LoRa Module-----------------------------------------//
void initLoRa()
{
  Serial.println("LoRa Sender Test");
  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);
  
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initializing OK!");
  display.setCursor(0,10);
  display.print("LoRa Initializing OK!");
  display.display();
  delay(2000);
}

//-------------------Initialize BMP280 Sensor-------------------------------------------//
void initBMP(){
  Serial.println(F("Initializing BMP280 Sensor"));
  if (!bme.begin(0x76)) {  
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
  Serial.println(F("BMP280 is OK!"));
}

//--------------------Initialize DHT Sensor---------------------------------------------//
void initDHT()
{
  // Initialize DHT device.
  Serial.println(F("Initializing DHT Sensor"));
  dht.begin();
  Serial.println(F("DHT Sensor is OK!"));
}

//-----------------function for fetching DHT readings-----------------------------------//
void getDHTreadings()
{
  // Delay between measurements.
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Temperature=event.temperature;
    Serial.print(Temperature);
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Humidity = event.relative_humidity;
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }
}

//-----------------function for fetching BMP280 readings--------------------------------//
void getBMPreadings()
{
 Pressure=bme.readPressure()/133;
 Serial.print(F("Pressure: "));
 Serial.print(Pressure);
 Serial.println(F(" mmHg"));
}


//-----------------function for fetching All  readings at once--------------------------//
void getReadings()
{
  getDHTreadings();
  getBMPreadings();
  delay(5000);
}

//-------------------------Display Readings on OLED-------------------------------------//
void displayReadings()
{
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("LoRa SENDER 1 - Pune");
  display.setCursor(0,20);
  display.setTextSize(1);
  display.print("TEMPERATURE");
  display.setCursor(70,20);
  display.print(":");
  display.setCursor(80,20);
  display.print(Temperature);
  display.drawCircle(116,20,1, WHITE);
  display.setCursor(121,20);
  display.print("C");
  display.setCursor(0,37);
  display.print("HUMIDITY(%)");
  display.setCursor(70,37);
  display.print(":");
  display.setCursor(80,37);
  display.print(Humidity);
  display.setCursor(116,37);
  display.print("Rh");
  display.setCursor(0,55);
  display.print("PRESSURE");
  display.setCursor(50,55);
  display.print(":");
  display.setCursor(60,55);
  display.print(Pressure);
  display.setCursor(100,55);
  display.print("mmHg");
  display.display();
  Serial.print("Sending packet: ");
  Serial.println(readingID);
  readingID++;
}

//------------------Send data to receiver node using LoRa-------------------------//
void sendReadings() {
  LoRaMessage = String(readingID) + "/" + String(Temperature) + "&" + String(Humidity) + "#" + String(Pressure);
  //Send LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print(LoRaMessage);
  LoRa.endPacket();

  Serial.println("Packet Sent!");
  displayReadings();

  delay(10000);
}

void setup() 
{
  //initialize Serial Monitor
  Serial.begin(115200);
  initDHT();
  initBMP();
  initOLED();
  initLoRa();
}

void loop() {
  getReadings();
  sendReadings();
}

//============================ www.blog.make2explore.com ==============================//

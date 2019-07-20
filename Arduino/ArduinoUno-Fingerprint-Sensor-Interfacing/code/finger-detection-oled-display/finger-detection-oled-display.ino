// ------------------- www.make2explore.com-----------------------------------------------------------
// Project           - ArduinoUno-FingerprintSensor-interfacing
// Created By        - admin@make2explore.com
// Version - 1.0
// Software Platform - C++, Arduino Libraries
// Hardware Platform - Arduino Uno R3, DY50 Fingerprint Sensor (FPM10A clone), 0.96' OLED display.                 
// Sensors Used      - DS50 Fingerprint Sensor
// ---------------------------------------------------------------------------------------------------
// Before running this sketch, You have to run "enroll" sketch from 'Adafruit Fingerprint Sensors-
// Library' first, to enroll and save your fingerprint templates for authentication.
// You should/must enroll 5-6 different positions of finger for each User for faster recognition.
// Run this sketch after that, place finger smoothely on sensor for greater accuracy

#include <Arduino.h>                  // If you're using Arduino IDE comment this include
                                      // used in PlatformIO
#include <Wire.h>                     // wire library for OLED display i2c
#include <Adafruit_GFX.h>             // graphics library for OLED
#include <Adafruit_SSD1306.h>         // OLED SS1306 display library
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET); 

#include <Adafruit_Fingerprint.h>     // Adafruit_Fingerprint sensor library
#include <SoftwareSerial.h>           // SoftwareSerial used for fingerprint sensor
SoftwareSerial mySerial(2, 3);        // instance (Rx,Tx)

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int fingerprintID = 0;
String IDname;

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("Found ID #"); 
  Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); 
  Serial.println(finger.confidence);
  return finger.fingerID; 
}

// function for display default screen on OLED
void displayMainScreen(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(18,5);
  display.println("Place finger On");
  display.setCursor(45,15);
  display.setTextSize(1);
  display.println("Sensor");
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(47,26);
  display.println("- - -");  
  display.display();
  //delay(2000);
}

// Function for display greeting for authenticated user
void displayUserGreeting(String Name){
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(25,0);
  display.print("Hello");
  display.setCursor(18,15);
  display.setTextSize(2);
  display.print(Name); 
  display.display();
  delay(1000);
  fingerprintID = 0; 
}

void setup(){
  //Fingerprint sensor module setup
  Serial.begin(9600);
  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } 
  else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  //OLED display setup
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  //displays main screen
  displayMainScreen();
}

void loop(){
  displayMainScreen();
  fingerprintID = getFingerprintIDez();
  delay(50);
  if((fingerprintID == 1) || (fingerprintID == 2) || (fingerprintID == 3) || (fingerprintID == 4) || (fingerprintID == 5)){
    IDname = "User - 1";                      // 1-5 templates are enrolled for user1
    displayUserGreeting(IDname);           
  }  
  else if((fingerprintID == 6) || (fingerprintID == 7) || (fingerprintID == 8) || (fingerprintID == 9) || (fingerprintID == 10)){
    IDname = "User - 2";                      // 6-10 templates are enrolled for user1
    displayUserGreeting(IDname);            
  }
}
// -------------------------------------------- www.make2explore.com---------------------------------------------------------------

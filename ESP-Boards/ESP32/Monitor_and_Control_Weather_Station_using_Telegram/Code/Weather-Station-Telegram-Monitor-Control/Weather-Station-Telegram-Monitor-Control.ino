// ------------------- blog.make2explore.com---------------------------------------------------------------//
// Project           - ESP32 Weather Station + Telegram
// Created By        - blog@make2explore.com
// Version - 1.0
// Software          - C/C++, Arduino IDE, Libraries
// Hardware          - NODEMCU ESP32s, BMP280 Sensor, DHT22 Sensor, 4Channel Relay Module(5V)                
// Sensors Used      - BMP280 Atm Pressure Sensor, DHT22 Temperature Humidity Sensor
// Last Modified     - 01/09/2022 21:44:00
//
// -- Latest Testing on  - 01/09/22  ✅
//    - With Following Environments
//    - ESP32 Core Version : 2.0.1
//    - Universal Telegram Bot library  v. 1.3.0
//    - ArduinoJson Library v. 6.15.2
// -------------------------------------------------------------------------------------------------------//

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h> // Universal Telegram Bot Library written by Brian Lough
#include <ArduinoJson.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>                 // DHT Sensor Library
#include <Adafruit_BMP280.h>     // BMP280 Sensor Library

#define DHTTYPE  DHT22           // Type of DHT Sensor DHT22 (AM2302)
#define DHTPIN 25                // Digital pin connected to the DHT sensor
DHT dht(DHTPIN, DHTTYPE);        // creating dht Instance

// BMP280 connect to ESP32 I2C (GPIO 21 = SDA, GPIO 22 = SCL)
Adafruit_BMP280 bmp;             // Create bmp instance

// Replace with your network credentials
const char* ssid = "XXXXXXX";
const char* password = "XXXXXXX";

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you

#define CHAT_ID "XXXXXXXXXX"     // Your Chat ID

// Initialize Telegram BOT
#define BOTtoken "XXXXXXXXXX:YYYYYYYYYYYYYYYYYYYYYYY"  // your Bot Token (Got from Botfather)

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
// 
// client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
// Adding root certificate in Setup()

//Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

const int Relay1 = 26;           // Relay Pins Connected to GPIO's
bool Relay1State = HIGH;

const int Relay2 = 27;
bool Relay2State = HIGH;

const int Relay3 = 14;
bool Relay3State = HIGH;

const int Relay4 = 12;
bool Relay4State = HIGH;

// Get BMP280 & DHT22 sensor readings and return them as a String variable
  String getReadings(){
  float temperature, humidity, pressure, Altitude, heatIndex;
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  pressure = (bmp.readPressure())/133;
  Altitude = bmp.readAltitude(1012);
  heatIndex = dht.computeHeatIndex(temperature, humidity, false);
  
  String message = "Temperature: " + String(temperature) + " ºC \n\n";
  message += "Humidity: " + String (humidity) + " % \n\n";
  message += "Pressure: " + String (pressure) + " mmHg \n\n";
  message += "Altitude: " + String (Altitude) + " m \n\n";
  message += "Heat Index: " + String (heatIndex) + " ºC \n\n";
  return message;
}


//Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Welcome, " + from_name + ".\n\n";
      welcome += "Use the following command to get Weather Station current readings.\n\n";
      welcome += "/readings \n\n";
      welcome += "Use the following command to Control The Appliances.\n\n";
      welcome += "/relay1_ON or /relay1_OFF \n";
      welcome += "/relay2_ON or /relay2_OFF \n";
      welcome += "/relay3_ON or /relay3_OFF \n";
      welcome += "/relay4_ON or /relay4_OFF \n\n";
      welcome += "/all_Relay_ON or /all_Relay_OFF \n\n";
      welcome += "/RelayStatus For Status of Relays \n\n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/readings") {
      String readings = getReadings();
      bot.sendMessage(chat_id, readings, "");
    }

    if (text == "/relay1_ON") {
      bot.sendMessage(chat_id, "Relay1 State set to ON", "");
      Relay1State = LOW;
      digitalWrite(Relay1, Relay1State);
    }
    
    if (text == "/relay1_OFF") {
      bot.sendMessage(chat_id, "Relay1 State set to OFF", "");
      Relay1State = HIGH;
      digitalWrite(Relay1, Relay1State);
    }


    if (text == "/relay2_ON") {
      bot.sendMessage(chat_id, "Relay2 State set to ON", "");
      Relay2State = LOW;
      digitalWrite(Relay2, Relay2State);
    }
    
    if (text == "/relay2_OFF") {
      bot.sendMessage(chat_id, "Relay2 State set to OFF", "");
      Relay2State = HIGH;
      digitalWrite(Relay2, Relay2State);
    }


    if (text == "/relay3_ON") {
      bot.sendMessage(chat_id, "Relay3 State set to ON", "");
      Relay3State = LOW;
      digitalWrite(Relay3, Relay3State);
    }
    
    if (text == "/relay3_OFF") {
      bot.sendMessage(chat_id, "Relay3 State set to OFF", "");
      Relay3State = HIGH;
      digitalWrite(Relay3, Relay3State);
    }


    if (text == "/relay4_ON") {
      bot.sendMessage(chat_id, "Relay4 State set to ON", "");
      Relay4State = LOW;
      digitalWrite(Relay4, Relay4State);
    }
    
    if (text == "/relay4_OFF") {
      bot.sendMessage(chat_id, "Relay4 State set to OFF", "");
      Relay4State = HIGH;
      digitalWrite(Relay4, Relay4State);
    }

    if (text == "/all_Relay_ON") {
      bot.sendMessage(chat_id, "All Relay State set to ON", "");
      digitalWrite(Relay1, LOW);
      digitalWrite(Relay2, LOW);
      digitalWrite(Relay3, LOW);
      digitalWrite(Relay4, LOW);
    }

    if (text == "/all_Relay_OFF") {
      bot.sendMessage(chat_id, "All Relay State set to OFF", "");
      digitalWrite(Relay1, HIGH);
      digitalWrite(Relay2, HIGH);
      digitalWrite(Relay3, HIGH);
      digitalWrite(Relay4, HIGH);
    }

    //status command impl
    if (text == "/RelayStatus") {
      char buf[100]= "";
      unsigned int pins[] = {26, 27, 14, 12};
      unsigned int nums[] = {1, 2, 3, 4};
      for(int i=0;i<=3;i++)
      {
          if (digitalRead(pins[i])){
            sprintf(buf,"Relay %d is OFF \n",nums[i]);
          }
          else{
            sprintf(buf,"Relay %d is ON \n",nums[i]);
          }
          bot.sendMessage(chat_id, buf, ""); 
      } 
    }

  }
    
}


void setup() {
  Serial.begin(115200);

  pinMode(Relay1, OUTPUT);             //define Relay pins as Digital OUT
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);

  digitalWrite(Relay1, Relay1State);   // Relay operate at inverted logic, so When RelayPin is "HIGH" Relay is Off
  digitalWrite(Relay2, Relay2State);   // When "LOW" Relay is On
  digitalWrite(Relay3, Relay3State);   // Here initially we make all relays Off
  digitalWrite(Relay4, Relay4State);

  // Init BMP280 sensor
  if (!bmp.begin(0x76)) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }

  // Init DHT22 sensor
  dht.begin();

  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
  delay(5000);
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}

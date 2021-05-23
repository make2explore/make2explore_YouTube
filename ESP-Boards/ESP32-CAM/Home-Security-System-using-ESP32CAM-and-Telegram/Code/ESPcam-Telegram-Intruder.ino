// ---------------------------------- make2explore.com----------------------------------------------------//
// Project           - Home Security System using ESP32 CAM and Telegram App
// Created By        - blog@make2explore.com
// Version - 1.0
// Last Modified     - 22/02/2021 15:00:00 @admin
// Software          - C/C++, Arduino IDE, Libraries
// Hardware          - Ai Thinker ESP32 CAM, MQ6 Sensor, Flame Sensor, AM312 PIR Sensor               
// Sensors Used      - MQ6 LPG/Gas/Smoke Sensor, Flame Sensor, AM312 PIR Sensor
// Source Repo       - github.com/make2explore
// -------------------------------------------------------------------------------------------------------//

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_camera.h"
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "XXXXXXXXXXXXXX";
const char* password = "XXXXXXXXXXXXX";

// Use @myidbot or @userinfobot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can message you

String chatId = "XXXXXXXXXXX";

// Initialize Telegram BOT
String BOTtoken = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

bool sendPhoto = false;

WiFiClientSecure clientTCP;

UniversalTelegramBot bot(BOTtoken, clientTCP);

//CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

#define FLASH_LED_PIN 4
bool flashState = LOW;

// Motion Sensor
const byte motionSensor = 13;
bool motionDetected = false;
bool motionDetectEnable = false;

// Door Sensor
const byte door1 = 12;
const byte door2 = 2;
bool door1stat = true;
bool door2stat = true;
bool doorLockMonitor = false;

// fire sensor
bool fireDetectMonitor = false;
const byte firePin = 14;
bool fire = false;

// MQ6 Smoke LPG gas leak Detector
bool smokeDetectMonitor = false;
const byte smokePin = 15;
bool smoke = false;
 
int botRequestDelay = 1000;   // mean time between scan messages
long lastTimeBotRan;     // last time messages' scan has been done

void handleNewMessages(int numNewMessages);
String sendPhotoTelegram();

// ISR (Interrupt Service Routine) Indicates when motion is detected
static void IRAM_ATTR detectsMovement(void * arg){
  //Serial.println("MOTION DETECTED!!!");
  motionDetected = true;
}

void setup(){
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); 
  Serial.begin(115200);

  // Set All Sensors Pin As INPUT
  pinMode(door1, INPUT);
  pinMode(door2, INPUT);
  pinMode(firePin, INPUT);
  pinMode(smokePin, INPUT);
  
  // Camera Flash
  pinMode(FLASH_LED_PIN, OUTPUT);
  digitalWrite(FLASH_LED_PIN, flashState);
  
  // Initialize WiFi
  WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("ESP32-CAM IP Address: ");
  Serial.println(WiFi.localIP());

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  //init with high specs to pre-allocate larger buffers
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;  //0-63 lower number means higher quality
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;  //0-63 lower number means higher quality
    config.fb_count = 1;
  }
  
  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    delay(1000);
    ESP.restart();
  }

  // Drop down frame size for higher initial frame rate
  sensor_t * s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_CIF);  // UXGA|SXGA|XGA|SVGA|VGA|CIF|QVGA|HQVGA|QQVGA

  // PIR Motion Sensor mode INPUT_PULLUP
  //err = gpio_install_isr_service(0); 
  err = gpio_isr_handler_add(GPIO_NUM_13, &detectsMovement, (void *) 13);  
  if (err != ESP_OK){
    Serial.printf("handler add failed with error 0x%x \r\n", err); 
  }
  err = gpio_set_intr_type(GPIO_NUM_13, GPIO_INTR_POSEDGE);
  if (err != ESP_OK){
    Serial.printf("set intr type failed with error 0x%x \r\n", err);
  }

  delay(40000); // Give time for PIR and MQ6 Gas Sensor to get Stable
  Serial.printf("PIR Sensor Initilized!! \n");
}

void loop(){

  if (sendPhoto){
    Serial.println("Preparing photo");
    sendPhotoTelegram(); 
    sendPhoto = false; 
  }
  
  // If Door 1 is got Opened
  door1stat = digitalRead(door1);
  if(!door1stat && doorLockMonitor){
    bot.sendMessage(chatId, "Alert - Door1 Opened !!", "");
    Serial.println("Door1 Opened");
    door1stat = true
    rue;
  }

  // If Door 2 is got Opened
  door2stat = digitalRead(door2);
  if(!door2stat && doorLockMonitor){
    bot.sendMessage(chatId, "Alert - Door2 Opened !!", "");
    Serial.println("Door2 Opened");
    door2stat = true;
  }

  // If Fire got detected in house
  fire = digitalRead(firePin);
  if(!fire && fireDetectMonitor){
    bot.sendMessage(chatId, "Alert - Fire at house !!", "");
    Serial.println("Fire Detected");
    fire = false;
  }

  // If Smoke got detected in house
  smoke = digitalRead(smokePin);
  if(!smoke && smokeDetectMonitor){
    bot.sendMessage(chatId, "Alert - Something is Burning !!", "");
    Serial.println("Smoke Detected");
    smoke = false;
  }
  
  // If Intruder Motion got detected in house
  if(motionDetected && motionDetectEnable){
    bot.sendMessage(chatId, "Motion detected!!", "");
    Serial.println("Motion Detected");
    sendPhotoTelegram();
    motionDetected = false;
    delay(2000);
  }
  
  if (millis() > lastTimeBotRan + botRequestDelay){
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages){
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}

String sendPhotoTelegram(){
  const char* myDomain = "api.telegram.org";
  String getAll = "";
  String getBody = "";

  camera_fb_t * fb = NULL;
  fb = esp_camera_fb_get();  
  if(!fb) {
    Serial.println("Camera capture failed");
    delay(1000);
    ESP.restart();
    return "Camera capture failed";
  }  
  
  Serial.println("Connect to " + String(myDomain));

  if (clientTCP.connect(myDomain, 443)) {
    Serial.println("Connection successful");
    
    String head = "--make2explore\r\nContent-Disposition: form-data; name=\"chat_id\"; \r\n\r\n" + chatId + "\r\n--make2explore\r\nContent-Disposition: form-data; name=\"photo\"; filename=\"esp32-cam.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";
    String tail = "\r\n--make2explore--\r\n";

    uint16_t imageLen = fb->len;
    uint16_t extraLen = head.length() + tail.length();
    uint16_t totalLen = imageLen + extraLen;
  
    clientTCP.println("POST /bot"+BOTtoken+"/sendPhoto HTTP/1.1");
    clientTCP.println("Host: " + String(myDomain));
    clientTCP.println("Content-Length: " + String(totalLen));
    clientTCP.println("Content-Type: multipart/form-data; boundary=make2explore");
    clientTCP.println();
    clientTCP.print(head);
  
    uint8_t *fbBuf = fb->buf;
    size_t fbLen = fb->len;
    for (size_t n=0;n<fbLen;n=n+1024) {
      if (n+1024<fbLen) {
        clientTCP.write(fbBuf, 1024);
        fbBuf += 1024;
      }
      else if (fbLen%1024>0) {
        size_t remainder = fbLen%1024;
        clientTCP.write(fbBuf, remainder);
      }
    }  
    
    clientTCP.print(tail);
    
    esp_camera_fb_return(fb);
    
    int waitTime = 10000;   // timeout 10 seconds
    long startTimer = millis();
    boolean state = false;
    
    while ((startTimer + waitTime) > millis()){
      Serial.print(".");
      delay(100);      
      while (clientTCP.available()) {
        char c = clientTCP.read();
        if (state==true) getBody += String(c);        
        if (c == '\n') {
          if (getAll.length()==0) state=true; 
          getAll = "";
        } 
        else if (c != '\r')
          getAll += String(c);
        startTimer = millis();
      }
      if (getBody.length()>0) break;
    }
    clientTCP.stop();
    Serial.println(getBody);
  }
  else {
    getBody="Connected to api.telegram.org failed.";
    Serial.println("Connected to api.telegram.org failed.");
  }
  return getBody;
}

void handleNewMessages(int numNewMessages){
  Serial.print("Handle New Messages: ");
  Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++){
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != chatId){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String fromName = bot.messages[i].from_name;
    
    // Implementation of all the Commands from START
    if (text == "/flashOn") {
      flashState = true;
      bot.sendMessage(chat_id, "Flash is set to ON", "");
      digitalWrite(FLASH_LED_PIN, flashState);
    }

    if (text == "/flashOff") {
      flashState = false;
      bot.sendMessage(chat_id, "Flash is set to OFF", "");
      digitalWrite(FLASH_LED_PIN, flashState);
    }
    
    if (text == "/photo") {
      sendPhoto = true;
      bot.sendMessage(chat_id, "Capturing Photo...", "");
      Serial.println("New photo request");
    }

    if (text == "/EnableMotionAlert") {
       motionDetectEnable = true;
       bot.sendMessage(chat_id, "Motion Alert - Enabled", "");
       Serial.println("Enable the Motion alert");
    }
    
    if (text == "/DisableMotionAlert") {
       motionDetectEnable = false;
       bot.sendMessage(chat_id, "Motion Alert - Disabled", "");
       Serial.println("Disable the Motion alert");
    } 

    if (text == "/EnableDoorAlert") {
       doorLockMonitor = true;
       bot.sendMessage(chat_id, "Monitor Doors - Enabled", "");
       Serial.println("Enable the Monitor Doors");
    }
    
    if (text == "/DisableDoorAlert") {
       doorLockMonitor = false;
       bot.sendMessage(chat_id, "Monitor Doors - Disabled", "");
       Serial.println("Disable the Monitor Doors");
    }

    if (text == "/EnableFireAlert") {
       fireDetectMonitor = true;
       bot.sendMessage(chat_id, "Fire Detector - Enabled", "");
       Serial.println("Enable the Fire Detector");
    }
    
    if (text == "/DisableFireAlert") {
       fireDetectMonitor = false;
       bot.sendMessage(chat_id, "Fire Detector - Disabled", "");
       Serial.println("Disable the Fire Detector");
    }


    if (text == "/EnableSmokeAlert") {
       smokeDetectMonitor = true;
       bot.sendMessage(chat_id, "Smoke Detector - Enabled", "");
       Serial.println("Enable the Smoke Detector");
    }
    
    if (text == "/DisableSmokeAlert") {
       smokeDetectMonitor = false;
       bot.sendMessage(chat_id, "Smoke Detector - Disabled", "");
       Serial.println("Disable the Smoke Detector");
    }

    if (text == "/start"){
      String welcome = "Welcome to the make2explore ESP32-CAM Telegram bot.\n\n";
      welcome += "Use the following command to Setup the ESPCAM.\n\n";
      welcome += "/flashOn : Turn on the flash LED\n";
      welcome += "/flashOff : Turn off the flash LED\n";
      welcome += "/photo : takes a new photo\n\n";
      welcome += "/EnableMotionAlert : enables the motion Alert System\n";
      welcome += "/DisableMotionAlert : disables the motion Alert System\n\n";
      welcome += "/EnableDoorAlert : enables the door Open Alert System\n";
      welcome += "/DisableDoorAlert : disables the door Open Alert System\n\n";
      welcome += "/EnableSmokeAlert : enables the Smoke Alert System\n";
      welcome += "/DisableSmokeAlert : disables the Smoke Alert System\n\n";
      welcome += "/EnableFireAlert : enables the Fire Alert System\n";
      welcome += "/DisableFireAlert : disables the Fire Alert System\n\n";
      welcome += "You'll receive a photo whenever motion is detected.\n";
      bot.sendMessage(chatId, welcome, "Markdown");
    }
  }
}

// -------------------------------------# make2explore.comn #----------------------------------------------//

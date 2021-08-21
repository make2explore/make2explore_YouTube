// ---------------------------------- make2explore.com----------------------------------------------------//
// Project           - IoT Based Smart Lock- RFID + Fingerprint + PIN + OTP + MySQL DB
// Created By        - blog@make2explore.com
// Version - 1.0
// Last Modified     - 07/08/2021 15:00:00 @admin
// Software          - C/C++, Arduino IDE, ESP32 SDK, Libraries
// Hardware          - NodeMCU ESP32S, RC522 RFID Reader, FPM10A Fingerprint Sensor, Nextion Display, Solenoid Lock              
// Sensors Used      - RC522 RFID Reader, FPM10A Fingerprint Sensor
// Source Repo       - github.com/make2explore
// -------------------------------------------------------------------------------------------------------//

#include <WiFi.h>                   // include wifi related libraries
#include <NTPClient.h>              // Required for fetching current time
#include <HTTPClient.h>             // Required for HTTP POST/GET request, functions
#include <UniversalTelegramBot.h>   // Telegram BOT API library

#include <SPI.h>                    // Library required for RC522 RFID Reader
#include <MFRC522.h>                // Library of RC522 RFID Reader
#include "Nextion.h"                // Library of Nextion Display
#include <Adafruit_Fingerprint.h>   // Library of Fingerprint Sensor

#define SS_PIN 21                   // SDA/SS Pin of RFID Reader
#define RST_PIN 22                  // RESET Pin of RFID Reader
#define LockerPin 33                // Relay driving Solenoid lock is connected to this pin (Active Low)

// Replace with your network credentials
const char* ssid = "XXXXXXXXXXXXXX";
const char* password = "XXXXXXXXXXXXX";

// Website Details
// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://example.com/post_data.php";

// Keep this API Key value to be compatible with the PHP code provided in the Github. 
// If you change the apiKeyValue value, the PHP file /post-data.php also needs to have the same key 
String apiKeyValue = "XXXXXXXXXXX";

// Initialize Telegram BOT
#define BOTtoken "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"  // your Bot Token (Got from Botfather)

// Use @myidbot or @userinfobot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can message you
#define CHAT_ID "XXXXXXXXXXX"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

MFRC522 mfrc522(SS_PIN, RST_PIN);            // Create MFRC522 instance.
String user="";                              // String variable For Username
String data="";                              // String variable For PIN data from Nextion
String branchName = "Secure Co-Op Bank";
String userName ="";

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");


String RealT = "";
//Week Days
String weekDays[7]={"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

//Month names
String months[12]={"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};


Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2);        //Fingerprint sensor is connected to Serial port 2

// Prototypes of CallBack Functions

/*
 *******************************************************************
   Nextion component for page:  Home
 *******************************************************************
*/

NexText DateTimeH = NexText(0, 2, "DateTimeH");

/*
 *******************************************************************
   Nextion component for page:   ValidRFID
 *******************************************************************
*/
NexText DateTimeV = NexText(1, 2, "DateTimeV");
NexText ValNM = NexText(0, 1, "ValNM");


/*
 *******************************************************************
   Nextion component for page:   InvalidRFID
 *******************************************************************
*/
NexText DateTimeI = NexText(2, 2, "DateTimeI");
NexText InvalNM = NexText(2, 1, "InvalNM");

/*
 *******************************************************************
   Nextion component for page:   About
 *******************************************************************
*/
NexText DateTimeA = NexText(3, 2, "DateTimeA");

/*
 *******************************************************************
   Nextion component for page:   PinEnter
 *******************************************************************
*/
NexText DateTimeP = NexText(4, 21, "DateTimeP");
NexText PinDM1 = NexText(4, 19, "PinDM1");
NexText PinDM2 = NexText(4, 20, "PinDM2");

NexText PinT0 = NexText(4, 14, "PinT0");
NexText PinT1 = NexText(4, 15, "PinT1");
NexText PinT2 = NexText(4, 16, "PinT2");
NexText PinT3 = NexText(4, 17, "PinT3");

NexButton PinE = NexButton(4, 12, "PinE");

/*
 *******************************************************************
   Nextion component for page:   otpEnter
 *******************************************************************
*/
NexText DateTimeO = NexText(5, 21, "DateTimeO");
NexText otpDM1 = NexText(5, 19, "otpDM1");
NexText otpDM2 = NexText(5, 20, "otpDM2");

NexText otpT0 = NexText(5, 14, "otpT0");
NexText otpT1 = NexText(5, 15, "otpT1");
NexText otpT2 = NexText(5, 16, "otpT2");
NexText otpT3 = NexText(5, 17, "otpT3");

NexButton otpE = NexButton(5, 12, "PinE");

/*
 *******************************************************************
   Nextion component for page:   Welcome
 *******************************************************************
*/
NexText DateTimeW = NexText(6, 2, "DateTimeW");
NexText wTxt = NexText(6, 1, "wTxt");

/*
 *******************************************************************
   Nextion component for page:   Finger
 *******************************************************************
*/
NexText DateTimeFH = NexText(7, 1, "DateTimeFH");

/*
 *******************************************************************
   Nextion component for page:   FingerScan
 *******************************************************************
*/
NexText DateTimeFS = NexText(8, 1, "DateTimeFS");

/*
 *******************************************************************
   Nextion component for page:   ValidFinger
 *******************************************************************
*/
NexText DateTimeVF = NexText(9, 1, "DateTimeVF");

/*
 *******************************************************************
   Nextion component for page:   InvalidFinger
 *******************************************************************
*/
NexText DateTimeIF = NexText(10, 1, "DateTimeIF");



void PinEPopCallback(void *ptr);
void otpEPopCallback(void *ptr);


// Declare a Page objects 
NexPage Home         = NexPage(0, 0, "Home");
NexPage ValidRFID    = NexPage(1, 0, "ValidRFID");
NexPage InvalidRFID  = NexPage(2, 0, "InvalidRFID");
NexPage About        = NexPage(3, 0, "About");
NexPage PinEnter     = NexPage(4, 0, "PinEnter");
NexPage otpEnter     = NexPage(5, 0, "otpEnter");
NexPage Welcome      = NexPage(6, 0, "Welcome");
NexPage Finger       = NexPage(7, 0, "Finger");
NexPage FingerScan   = NexPage(8, 0, "FingerScan");
NexPage ValidFinger  = NexPage(9, 0, "ValidFinger");
NexPage InvalidFinger = NexPage(10, 0, "InvalidFinger");


char buffer[10] = {0};   // Buffer for collecting text values

// Register all objects to the touch event list.  

NexTouch *nex_listen_list[] = 
{
    &PinE,
    &otpE,
    NULL
};


//************send data to the website*************
void SendDataToWebsite(){
  //Check WiFi connection status
  Serial.println("Sending Data to Website");
  if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;    //Declare object of class HTTPClient
    
    GetDateTime();
    
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&userID=" + user  + "&userName=" + userName + "&branchName=" + branchName + "";
                          
                          
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);

    
    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
    
    
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}
//*****************send data to the website*********



//  Button components pop callback functions - for PinEntry

void PinEPopCallback(void *ptr)
{
    mfrc522.PICC_HaltA(); // Stop reading
    String s = "";

    memset(buffer, 0, sizeof(buffer));
    
    PinT0.getText(buffer, sizeof(buffer));
    s += buffer;
    PinT1.getText(buffer, sizeof(buffer));
    s += buffer;
    PinT2.getText(buffer, sizeof(buffer));
    s += buffer;
    PinT3.getText(buffer, sizeof(buffer));
    s += buffer;

    Serial.print("Entered Pin = ");
    Serial.println(s);

    if (s == "1234" && user == "user1"){
      PinDM1.setText("Entered PIN is : VALID");
      PinDM2.setText("Now You'll Receive OTP on Telegram");

      int randNumber = random(1000, 9999);
      data = String(randNumber);
      String msg = "OTP for IoT Based Smart Locker -" + data + "\n";
      msg += "valid for one Time only";
      bot.sendMessage(CHAT_ID, msg, "");
      //OTP for IoT Based Smart Locker - {{PARTICLE_EVENT_VALUE}} valid for one Time only
      delay(1000);

      otpEnter.show();
      GetDateTime();
      DateTimeO.setText(RealT.c_str());
      delay(1000);
    }
    else if (s == "5678" && user == "user2"){
      PinDM1.setText("Entered PIN is : VALID");
      PinDM2.setText("Now You'll Receive OTP on Telegram");

      int randNumber = random(1000, 9999);
      data = String(randNumber);
      String msg = "OTP for IoT Based Smart Locker -" + data + "\n";
      msg += "valid for one Time only";
      bot.sendMessage(CHAT_ID, msg, "");
      //Particle.publish("telegramWebhook", data, PRIVATE);
      delay(1000);

      otpEnter.show();
      GetDateTime();
      DateTimeO.setText(RealT.c_str());
      delay(1000);
    }
    else {
      PinDM2.setText("Invalid PIN : Access Denied");
      delay(2000);
      printNormalModeMessage(); 
    }
}

//  Button components pop callback functions - for otpEntry

void otpEPopCallback(void *ptr){

    mfrc522.PICC_HaltA(); // Stop reading
    String o = "";

    memset(buffer, 0, sizeof(buffer));
    
    otpT0.getText(buffer, sizeof(buffer));
    o += buffer;
    otpT1.getText(buffer, sizeof(buffer));
    o += buffer;
    otpT2.getText(buffer, sizeof(buffer));
    o += buffer;
    otpT3.getText(buffer, sizeof(buffer));
    o += buffer;

    
    if (o == data && user == "user1"){
      otpDM1.setText("Step III passed : Valid OTP");
      otpDM2.setText("User : Access Granted");
      delay(2000);
      digitalWrite(LockerPin, LOW);
      Welcome.show();
      GetDateTime();
      DateTimeW.setText(RealT.c_str());
      wTxt.setText("Hello Aakash Gagan");
      userName = "Aakash Gagan";
      //char buf[128];
      //snprintf(buf, sizeof(buf), "[\"%s\",\"%s\"]", "User ID : 1", "Jason"); 
      SendDataToWebsite();
      delay(5000);
      digitalWrite(LockerPin, HIGH);
      printNormalModeMessage();
    }
    else if (o == data && user == "user2"){
      otpDM1.setText("Step III passed : Valid OTP");
      otpDM2.setText("User : Access Granted");
      delay(2000);
      digitalWrite(LockerPin, LOW);
      Welcome.show();
      GetDateTime();
      DateTimeW.setText(RealT.c_str());
      wTxt.setText("Hello Ravi Bhaskar");
      userName = "Ravi Bhaskar";
      //char buf[128];
      //snprintf(buf, sizeof(buf), "[\"%s\",\"%s\"]", "User ID : 2", "Ravi"); 
      SendDataToWebsite();
      delay(5000);
      digitalWrite(LockerPin, HIGH);
      printNormalModeMessage();
    }
    else {
      otpDM1.setText("Invalid OTP : Access Denied");
      otpDM2.setText("Try Again");
      delay(3000);
      printNormalModeMessage(); 
    }

    digitalWrite(LockerPin, HIGH);

}

void initRFID(){
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
}


void initFingerSensor(){
  
  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }

  finger.getTemplateCount();

  if (finger.templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  }
  else {
    Serial.println("Waiting for valid finger...");
      Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  }
 }
}
/* Function which Scans RFID Tags/Cards and Identify the User */

void ScanRFID(){

    if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
      return;
    }
    if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
      return;
    }

    String content= "";
    user="";
    for (byte i = 0; i < mfrc522.uid.size; i++) 
    {
      content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
      content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }

    content.toUpperCase();
    mfrc522.PICC_HaltA(); // Stop reading

    if (content.substring(1) == "B9 C7 3D 8D") //change here the UID of the card/cards that you want to give access
    {
      user = "user1";
      ValidRFID.show();
      GetDateTime();
      DateTimeV.setText(RealT.c_str());
      ValNM.setText("Welcome Aakash !");
      delay(3000);
      Finger.show();
      DateTimeFH.setText(RealT.c_str());
      delay(2000);
      int fingerID = getFingerprintID();
      
      if((fingerID == 1) || (fingerID == 2) || (fingerID == 3) || (fingerID == 4)){
        ValidFinger.show();
        DateTimeVF.setText(RealT.c_str());
        delay(2000);
        PinEnter.show();
        DateTimeP.setText(RealT.c_str());
      }else{
        InvalidFinger.show();
        DateTimeIF.setText(RealT.c_str());
        delay(2000);
        printNormalModeMessage();       
      }
    }
    else if(content.substring(1) == "FE CB 37 5B"){
      user = "user2";
      ValidRFID.show();
      GetDateTime();
      DateTimeV.setText(RealT.c_str());
      ValNM.setText("Welcome ravi !");
      delay(3000);
      Finger.show();
      DateTimeFH.setText(RealT.c_str());
      delay(2000);
      int fingerID = getFingerprintID();
      
      if((fingerID == 5) || (fingerID == 6) || (fingerID == 7) || (fingerID == 8)){
        ValidFinger.show();
        DateTimeVF.setText(RealT.c_str());
        delay(2000);
        PinEnter.show();
        DateTimeP.setText(RealT.c_str());
      }else{
        InvalidFinger.show();
        DateTimeIF.setText(RealT.c_str());
        delay(2000);
        printNormalModeMessage();       
      }
    }
    else {
      InvalidRFID.show();
      DateTimeI.setText(RealT.c_str());
      InvalNM.setText("Access Denied !!");
      delay(3000);
      printNormalModeMessage();
    }

}

// This function is for Display default initial screen

void printNormalModeMessage() {
  Home.show();
  DateTimeH.setText(RealT.c_str());
  delay(150);
}


//********************Get the Fingerprint ID******************
int getFingerprintID() {
  checkAgain:
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image taken");
      GetDateTime();
      FingerScan.show();
      DateTimeFS.setText(RealT.c_str());
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.println("No finger detected");
      goto checkAgain;
      return 0;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      return -2;
    case FINGERPRINT_IMAGEFAIL:
      //Serial.println("Imaging error");
      return -2;
    default:
      //Serial.println("Unknown error");
      return -2;
  }
  // OK success!
  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      //Serial.println("Image too messy");
      return -1;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      return -2;
    case FINGERPRINT_FEATUREFAIL:
      //Serial.println("Could not find fingerprint features");
      return -2;
    case FINGERPRINT_INVALIDIMAGE:
      //Serial.println("Could not find fingerprint features");
      return -2;
    default:
      //Serial.println("Unknown error");
      return -2;
  }
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    //Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
    return -2;
  } else if (p == FINGERPRINT_NOTFOUND) {
    //Serial.println("Did not find a match");
    return -1;
  } else {
    //Serial.println("Unknown error");
    return -2;
  }   
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence); 
 
  return finger.fingerID;
}

// Connect to Wi-Fi
void connectWifi(){
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

//Init NTPclient to get time
void initNTP(){
  // Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 1 * 60 * 60 = 3600
  // GMT +5 = 5 * 60 * 60 = 18000
  // GMT -1 = -1 *60 * 60 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(19080);
}

/* Function which sets current date time */

void GetDateTime(){
  timeClient.update();

  unsigned long epochTime = timeClient.getEpochTime();
  
  String formattedTime = timeClient.getFormattedTime();

  String weekDay = weekDays[timeClient.getDay()];

  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime); 

  int monthDay = ptm->tm_mday;

  int currentMonth = ptm->tm_mon+1;

  String currentMonthName = months[currentMonth-1];

  int currentYear = ptm->tm_year+1900;

  RealT = weekDay + " " + currentMonthName + " " + String(monthDay) + " " + formattedTime + " " + String(currentYear);

  //Serial.println(RealT);
  delay(1000);
}

void setup() {
  Serial.begin(115200);
  Serial.println("IoT based Smart Lock V2.0");
  delay(1000);

  /* Initialize the display and other peripherals */
  Serial.println("Setting up WiFi");
  connectWifi();
  delay(500);

  Serial.println("Fetching Time");
  initNTP();
  delay(250);
 
  pinMode(LockerPin, OUTPUT);           // Seting Relay PIN as Output
  digitalWrite(LockerPin, HIGH);        // Relay PIN is active LOW

  Serial.println("Setting up RFID");
  initRFID();
  delay(500);

  Serial.println("Setting up FingerPrint Sensor");
  initFingerSensor();
  delay(500);

  Serial.println("Setting Up Telegram API");
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org

  Serial.println("Setting up Nextion Display");
  nexInit();
  delay(500);

  /* Register the pop event callback function of the current button0 component. */
  PinE.attachPop(PinEPopCallback);
  otpE.attachPop(otpEPopCallback);

  printNormalModeMessage(); // Default initial screen
  GetDateTime();
  DateTimeH.setText(RealT.c_str());
  delay(1000);
}

void loop() {
    /*
     * When a pop or push event occured every time, 
     * the corresponding component[right page id and component id] in touch event list will be asked.
     */
  nexLoop(nex_listen_list);
  ScanRFID();
}
/*========================================= info@make2explore.com ===============================================================*/

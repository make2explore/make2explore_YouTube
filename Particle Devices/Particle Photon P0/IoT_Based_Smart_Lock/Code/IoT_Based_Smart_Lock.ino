// ---------------------------------- make2explore.com----------------------------------------------------//
// Project           - IoT Based Smart Lock- RFID + PIN + OTP + Google Sheet
// Created By        - blog@make2explore.com
// Version - 1.0
// Last Modified     - 23/06/2021 15:00:00 @admin
// Software          - C/C++, Particle Workbench, Libraries
// Hardware          - Particle Photon P0, RC522 RFID Reader, Nextion Display, Solenoid Lock              
// Sensors Used      - RC522 RFID Reader
// Source Repo       - github.com/make2explore
// -------------------------------------------------------------------------------------------------------//

#include "Nextion.h"                // Library of Nextion Display
#include "MFRC522.h"                // Library for RC522 RFID Reader

#define SS_PIN A2                   // SDA/SS Pin of RFID Reader
#define RST_PIN D2                  // RESET Pin of RFID Reader
#define LockerPin D4                // Relay driving Solenoid lock is connected to this pin (Active Low)

MFRC522 mfrc522(SS_PIN, RST_PIN);	// Create MFRC522 instance.
String user="";                     // For Username
String data="";                     // PIN data from Nextion


// Prototypes of CallBack Functions

USARTSerial& nexSerial = Serial1;

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
NexText DateTimeV = NexText(0, 2, "DateTimeV");
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


char buffer[10] = {0};   // Buffer for collecting text values

// Register all objects to the touch event list.  

NexTouch *nex_listen_list[] = 
{
    &PinE,
    &otpE,
    NULL
};


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
    

    if (s == "1234" && user == "user1"){
      PinDM1.setText("Entered PIN is : VALID");
      PinDM2.setText("Now You'll Receive OTP on Telegram");

      int randNumber = random(1000, 9999);
      data = String(randNumber);
      Particle.publish("telegramWebhook", data, PRIVATE);
      delay(6000);

      otpEnter.show();
      SetDateTime();
      delay(1000);
    }
    else if (s == "5678" && user == "user2"){
      PinDM1.setText("Entered PIN is : VALID");
      PinDM2.setText("Now You'll Receive OTP on Telegram");

      int randNumber = random(1000, 9999);
      data = String(randNumber);
      Particle.publish("telegramWebhook", data, PRIVATE);
      delay(6000);

      otpEnter.show();
      SetDateTime();
      delay(1000);
    }
    else {
      PinDM2.setText("Invalid PIN : Access Denied");
      delay(3000);
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
      SetDateTime();
      wTxt.setText("Hello Jason");
      char buf[128];
      snprintf(buf, sizeof(buf), "[\"%s\",\"%s\"]", "User ID : 1", "Jason"); 
      Particle.publish("LockerEntry", buf, PRIVATE);
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
      SetDateTime();
      wTxt.setText("Hello Ravi");
      char buf[128];
      snprintf(buf, sizeof(buf), "[\"%s\",\"%s\"]", "User ID : 2", "Ravi"); 
      Particle.publish("LockerEntry", buf, PRIVATE);
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

void setup() {
  
  /* Initialize the display and other peripherals */
  nexInit();
  pinMode(LockerPin, OUTPUT);
  digitalWrite(LockerPin, HIGH);

	mfrc522.setSPIConfig();
	mfrc522.PCD_Init();	// Init MFRC522 card

  /* Register the pop event callback function of the current button0 component. */
  PinE.attachPop(PinEPopCallback);
  otpE.attachPop(otpEPopCallback);
  Particle.syncTime(); /*sync to current time to the particle cloud*/

  printNormalModeMessage(); // Default initial screen
  SetDateTime();
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

/* Function which sets current date time */

void SetDateTime(){
  Time.zone(+5.30);
  DateTimeH.setText(Time.timeStr());
  DateTimeV.setText(Time.timeStr());
  DateTimeI.setText(Time.timeStr());
  DateTimeA.setText(Time.timeStr());
  DateTimeP.setText(Time.timeStr());
  DateTimeO.setText(Time.timeStr());
  DateTimeW.setText(Time.timeStr());
  delay(250);
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
      SetDateTime();
      ValNM.setText("Welcome Jason !");
      delay(3000);
      PinEnter.show();
      SetDateTime();
    }
    else if(content.substring(1) == "FE CB 37 5B"){
      user = "user2";
      ValidRFID.show();
      SetDateTime();
      ValNM.setText("Welcome Ravi !");
      delay(3000);
      PinEnter.show();
      SetDateTime();
    }
    else {
      InvalidRFID.show();
      SetDateTime();
      InvalNM.setText("Access Denied !!");
      delay(3000);
      printNormalModeMessage();
    }

}

// This function is for Display default initial screen

void printNormalModeMessage() {
  Home.show();
  SetDateTime();
  delay(150);
}

/*========================================= info@make2explore.com ===============================================================*/
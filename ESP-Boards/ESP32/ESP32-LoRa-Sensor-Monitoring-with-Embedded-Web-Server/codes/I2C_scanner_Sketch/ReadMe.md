i2c_Scanner Sketch  

1. i2c_Scanner Sketch will scan for connected I2C devices to Arduino/ESP/ any Arduino core dev Boards.  

2. Sketch will then display the (i2c) address of connected devices


Known Issues-  ["Could not find a valid BMP280 sensor, check wiring!"]  

This issue is of I2C connection between ESP32 TTGO LoRa and BMP280, Please follow the steps below to troubleshoot this problem

1. First check wiring (SCA, SDA lines) for correct connection as per circuit diagram given in GitHub link.
2. Then find out the *I2C address* of your BMP280 sensor using "i2c Scanner sketch" which is also given in GitHub link. 
3. After that in LoRa_Sender_Web_Server.ino sketch go to Line 123 which looks like following
  
if (!bme.begin(0x76)) {  

and change the address (0x76) to your sensors address...(which you've got from i2cscanner)

****If above does not work and if your sensors address is different than (0x76, 0x77) then do the following
1. Go to Arduino library folder "C:\Users\USERNAME\Documents\Arduino\libraries\Adafruit_BMP280_Library"
2. Edit the header file "Adafruit_BMP280.h" and in line 32,33 change default I2C addresses as per your sensors i2c address.

GitHub Link:
▶️ https://bit.ly/3bLBCh8
also 
*other code links are given in the description of video. 

If you still facing any issues, Feel free to contact us through email  
📩 - info@make2explore.com
or Search "@make2explore"  on Telegram 🚀

We'd ❤️ love to help.
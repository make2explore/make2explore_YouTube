# ------------------- www.make2explore.com-----------------------------------
# Project - Raspberry Pi ESP32 MicroPython MQTT DHT22 Tutorial
# Created By - admin@make2explore.com
# Version - 1.0
# Software Platform - MicroPython, MQTT, Python3, uPyCraftV1.1 IDE
# Hardware Platform - Raspberry Pi3B+, ESP32(NODEMCU)
# Sensors Used - DHT22
# ---------------------------------------------------------------------------
# Boot code for ESP32 loaded using uPyCraft IDE in device folder of directory
# Replace your network credentials in ''

import network
import esp

ssid = 'YOUR SSID'
password = 'PASSWORD'


station = network.WLAN(network.STA_IF)

station.active(True)
station.connect(ssid, password)

while station.isconnected() == False:
  pass

print('Connection successful')
print(station.ifconfig())
#-------------------------------------------------------------------------------

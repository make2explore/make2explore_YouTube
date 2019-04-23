# ------------------- www.make2explore.com-----------------------------------
# Project - Raspberry Pi ESP32 MicroPython MQTT DHT22 Tutorial
# Created By - admin@make2explore.com
# Version - 1.0
# Software Platform - MicroPython, MQTT, Python3
# Hardware Platform - Raspberry Pi3B+, ESP32(NODEMCU)
# Sensors Used - DHT22
# ---------------------------------------------------------------------------
# RaspberryPi python code
# By default I2C in Rpi is disabled, you have to enable it using raspi-config command
# Run this script as python3 to avoid any version error

import paho.mqtt.client as mqtt
import Adafruit_SSD1306
from PIL import Image, ImageDraw, ImageFont

disp = Adafruit_SSD1306.SSD1306_128_32(rst=0)
disp.begin()
FONT_PATH = '/usr/share/fonts/truetype/piboto/PibotoCondensed-Regular.ttf'
FONT = ImageFont.truetype(FONT_PATH, 22)

def display_data(t, h):
    image = Image.new('1', (disp.width, disp.height))
    draw = ImageDraw.Draw(image)
    # Draw temperature / Humidity values.
    draw.text((0, 8), '{0}°C'.format(t),  font=FONT, fill=255)
    draw.text((71, 8), '{0}%'.format(h), font=FONT, fill=255)
    # Draw bar charts.
    draw.rectangle((0, 0, 50, 8), outline=255, fill=0)
    draw.rectangle((71, 0, 121, 8), outline=255, fill=0)
    draw.rectangle((0, 0, t / 100.0 * 50, 8), outline=255, fill=255)
    draw.rectangle((71, 0, 71 + (h / 100.0 * 50), 8), outline=255, fill=255)
    # Send to OLED display.
    disp.clear()
    disp.image(image)
    disp.display()

        # Callback fires when conected to MQTT broker.
def on_connect(client, userdata, flags, rc):
    print('Connected with result code {0}'.format(rc))
    # Subscribe (or renew if reconnect).
    client.subscribe('temp_humidity')


# Callback fires when a published message is received.
def on_message(client, userdata, msg):
        # Decode temperature and humidity values from binary message paylod.
    t,h = [float(x) for x in msg.payload.decode("utf-8").split(',')]
    print('{0}°C {1}%'.format(t, h))
    display_data(t, h)  # Display data on OLED display.


client = mqtt.Client()
client.on_connect = on_connect  # Specify on_connect callback
client.on_message = on_message  # Specify on_message callback
client.connect('localhost', 1883, 60)  # Connect to MQTT broker (also running on Pi).

# Processes MQTT network traffic, callbacks and reconnections. (Blocking)
client.loop_forever()
#-------------------------------------------------------------------------------------------

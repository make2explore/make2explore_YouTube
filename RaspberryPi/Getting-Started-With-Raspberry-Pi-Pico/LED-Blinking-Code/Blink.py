# ---------------------------------- make2explore.com----------------------------------------------------//
# Project           - Getting Started With Raspberry Pi Pico - LED Blinking
# Created By        - blog@make2explore.com
# Version - 1.0
# Last Modified     - 03/18/2021 23:00:00 @admin
# Software          - MicroPython, Python
# Hardware          - Raspberry Pi Pico with RP2040              
# Source Repo       - github.com/make2explore
# -------------------------------------------------------------------------------------------------------//

import machine     #import machine library where all GPIO and Pins related functions are defined
import utime       #import utime library where all timing related functions are defined, like delays, sleep etc

led_onboard = machine.Pin(25, machine.Pin.OUT)  # This line defines an object for on board LED, and Set The GPIO Direction

while True:                                     # Infinite/Forever Loop
    led_onboard.value(1)                        # Pass value "1" HIGH to turn ON The LED
    utime.sleep(2)                              # 2 Second Delay for LED ON
    led_onboard.value(0)                        # Pass value "0" LOW to turn OFF The LED
    utime.sleep(1)                              # 1 Second Delay for LED OFF
    
    
    
# ------------------------------------make2explore.com----------------------------------------------------//
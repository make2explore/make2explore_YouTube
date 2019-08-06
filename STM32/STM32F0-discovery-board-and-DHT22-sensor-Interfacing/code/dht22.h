// ------------------- www.make2explore.com-----------------------------------------------------------
// Project           - Tutorial - STM32F0 discovery board and DHT22 sensor Interfacing
// Created By        - admin@make2explore.com
// Version - 1.0
// Software Platform - C, Keil uVision5
// Hardware Platform - STM32F040 Discovery Board             
// Sensors/Modules   - DHT22 Temp+Humidity Sensor, FTDI module -usb to UART
// ---------------------------------------------------------------------------------------------------
// DHT22 Sensor function header file

/* Define to prevent recursive inclusion -----------------------------------*/
#ifndef __DHT22_H
#define __DHT22_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ----------------------------------------------------------------*/
#include "stm32f0xx.h"

/* defines -----------------------------------------------------------------*/
#define DHT22_DATA_PIN  (1<<10)
#define DHT22_GPIO		GPIOC
#define DHT22_TIM		TIM2

/* variables ---------------------------------------------------------------*/
uint8_t DHT22data[6];

/* functions prototypes ----------------------------------------------------*/
void DHT22pinIn(void);
void DHT22pinOut(void);
void DHT22_Init(void);
void DHT22_Read(void);
float DHT22getTemperature(void);
float DHT22getHumidity(void);
float convertCtoF(float cTemperature);

#ifdef __cplusplus
}
#endif

#endif
// -------------------------------------------- www.make2explore.com-------------------------------------------------//
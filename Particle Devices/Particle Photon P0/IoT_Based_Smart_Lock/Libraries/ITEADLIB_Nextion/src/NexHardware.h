/**
* @file NexHardware.h
*
* The definition of base API for using Nextion. 
*
* @author  Wu Pengfei (email:<pengfei.wu@itead.cc>)
* @date    2015/8/11
* @copyright 
* Copyright (C) 2014-2015 ITEAD Intelligent Systems Co., Ltd. \n
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation; either version 2 of
* the License, or (at your option) any later version.
*
* Port for Particle platform (particle.io)
* by BSpranger & ScruffR (Dec. 2015)
*/

#ifndef __NEXHARDWARE_H__
#define __NEXHARDWARE_H__
#if defined(SPARK)
#include "application.h"
#else
#include "Arduino.h"
#endif
#include "NexConfig.h"
#include "NexCommands.h"
#include "NexTouch.h"

//extern "C" char* itoa(int a, char* buffer, unsigned char radix);
//extern "C" char* utoa( unsigned int value, char* buffer, unsigned char radix );
/**
 * @addtogroup CoreAPI 
 * @{ 
 */

/**
 * Init Nextion and setup the serial port
 * 
 * @return true if success, false for failure. 
 */
bool nexInit(uint32_t baudrate = 9600);

/**
 * Listen touch event and calling callbacks attached before.
 * 
 * Supports push and pop at present. 
 *
 * @param nex_listen_list - index to Nextion Components list. 
 * @return none. 
 *
 * @warning This function must be called repeatedly to response touch events
 *  from Nextion touch panel. Actually, you should place it in your loop function. 
 */
void nexLoop(NexTouch *nex_listen_list[]);

/**
 * @}
 */

bool      recvRetNumber(uint32_t *number, uint32_t timeout = 100);
uint16_t  recvRetString(char *buffer, uint16_t len, uint32_t timeout = 100);
void      sendCommand(const char* cmd);
void      sendSkript(const char* cmd, bool noRR = false);
bool      recvRetCommandFinished(uint32_t timeout = 100);
          
bool      sendCurrentPageId(uint8_t* pageId);
bool      setCurrentBrightness(uint8_t dimValue, bool setDefault = false);
bool      setDefaultBaudrate(uint32_t baudRate);
bool      setBaudrate(uint32_t baudrate);
void      sendRefreshAll(void);

bool      NexGetValue(const char* objName, const char* valueType, uint32_t* value);
bool      NexSetValue(const char* objName, const char* valueType, uint32_t value);
uint16_t  NexGetString(const char* objName, const char* valueType, char* text, uint16_t len);
bool      NexSetString(const char* objName, const char* valueType, const char* text);

#endif /* #ifndef __NEXHARDWARE_H__ */

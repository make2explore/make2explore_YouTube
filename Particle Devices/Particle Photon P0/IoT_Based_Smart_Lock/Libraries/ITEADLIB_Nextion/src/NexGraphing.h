/**
* @file NexGraphing.h
*
* The definition of base API for basic graphics on Nextion. 
*
* @author  Andreas Rothenw�nder (aka ScruffR <http://community.particle.io>)
* @date    2015/12
* @copyright 
* Copyright (C) 2014-2015 ITEAD Intelligent Systems Co., Ltd. \n
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation; either version 2 of
* the License, or (at your option) any later version.
*/

#ifndef __NEXGRAPHING_H__
#define __NEXGRAPHING_H__
#if defined(SPARK)
#include "application.h"
#else
#include "Arduino.h"
#endif
#include "NexConfig.h"
#include "NexHardware.h"

/**
 * @addtogroup CoreAPI 
 * @{ 
 */
typedef enum
{
  NexCROP = 0,
  NexSOLID = 1,
  NexIMAGE = 2
} NexBACKGROUND_t;

typedef enum
{
  NexTOPLEFT = 0,
  NexCENTER = 1,
  NexBOTTOMRIGHT = 2
} NexTEXTALIGN_t;

void clearScreen(uint32_t color = 0xFFFFFF);
void plot(uint32_t x, uint32_t y, uint32_t color = 0);
void drawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color = 0);
void drawRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color = 0);
void fillRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color = 0);
void drawRectAbs(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color = 0);
void fillRectAbs(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color = 0);
void drawCircle(uint32_t x, uint32_t y, uint32_t r, uint32_t color = 0);
void drawPic(uint32_t x, uint32_t y, uint32_t picID = 0);
void cropPic(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t picID = 0);
void drawText(uint32_t x, uint32_t y, uint32_t w, uint32_t h, NexTEXTALIGN_t centerX, NexTEXTALIGN_t centerY,
     uint32_t fontID, uint32_t fontColor, uint32_t backColor, NexBACKGROUND_t backStyle,
     const char* text);
void drawTextAbs(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, NexTEXTALIGN_t centerX, NexTEXTALIGN_t centerY,
     uint32_t fontID, uint32_t fontColor, uint32_t backColor, NexBACKGROUND_t backStyle,
     const char* text);

/**
 * @}
 */


#endif /* #ifndef __NEXGRAPHING_H__ */

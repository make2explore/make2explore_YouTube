/**
* @file NexGraphing.cpp
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

#include "NexGraphing.h"


/*
 * Receive uint32_t data. 
 * 
 * @param number - save uint32_t data. 
 * @param timeout - set timeout time. 
 *
 * @retval true - success. 
 * @retval false - failed.
 *
 */

inline void swap(uint32_t* v1, uint32_t* v2)
{
  uint32_t dmy;
  dmy = *v2;
  *v2 = *v1;
  *v1 = dmy;
}

void clearScreen(uint32_t color)
{
  char cmd[16] = "cls ";
  utoa(color, &cmd[strlen(cmd)], 10);
  sendCommand(cmd);
}

void plot(uint32_t x, uint32_t y, uint32_t color)
{
  drawLine(x, y, x, y, color);
}

void drawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color)
{
  char cmd[32] = "line ";

  if (x1 > x2) swap(&x1, &x2);
  if (y1 > y2) swap(&y1, &y2);
  utoa(x1, &cmd[strlen(cmd)], 10);
  strcat(cmd, ",");
  utoa(y1, &cmd[strlen(cmd)], 10);
  strcat(cmd, ",");
  utoa(x2, &cmd[strlen(cmd)], 10);
  strcat(cmd, ",");
  utoa(y2, &cmd[strlen(cmd)], 10);
  strcat(cmd, ",");
  utoa(color, &cmd[strlen(cmd)], 10);
  sendCommand(cmd);
}

void drawRectAbs(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color)
{
  char cmd[32] = "draw ";

  if (x1 > x2) swap(&x1, &x2);
  if (y1 > y2) swap(&y1, &y2);
  utoa(x1, &cmd[strlen(cmd)], 10);
  strcat(cmd, ",");
  utoa(y1, &cmd[strlen(cmd)], 10);
  strcat(cmd, ",");
  utoa(x2-1, &cmd[strlen(cmd)], 10);
  strcat(cmd, ",");
  utoa(y2-1, &cmd[strlen(cmd)], 10);
  strcat(cmd, ",");
  utoa(color, &cmd[strlen(cmd)], 10);
  sendCommand(cmd);
}
void drawRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color)
{
  drawRectAbs(x, y, x + w, y + h, color);
}

void fillRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color)
{
  char cmd[32] = "fill ";

  utoa(x, &cmd[strlen(cmd)], 10);
  strcat(cmd, ",");
  utoa(y, &cmd[strlen(cmd)], 10);
  strcat(cmd, ",");
  utoa(w, &cmd[strlen(cmd)], 10);
  strcat(cmd, ",");
  utoa(h, &cmd[strlen(cmd)], 10);
  strcat(cmd, ",");
  utoa(color, &cmd[strlen(cmd)], 10);
  sendCommand(cmd);
}
void fillRectAbs(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color)
{
  if (x1 > x2) swap(&x1, &x2);
  if (y1 > y2) swap(&y1, &y2);
  fillRect(x1, y1, x2-x1, y2-y1, color);
}

void drawCircle(uint32_t x, uint32_t y, uint32_t r, uint32_t color)
{
  char cmd[32] = "cir ";
  
  utoa(x, &cmd[strlen(cmd)], 10);
  strcat(cmd, ",");
  utoa(y, &cmd[strlen(cmd)], 10);
  strcat(cmd, ",");
  utoa(r, &cmd[strlen(cmd)], 10);
  strcat(cmd, ",");
  utoa(color, &cmd[strlen(cmd)], 10);
  sendCommand(cmd);
}

void drawPic(uint32_t x, uint32_t y, uint32_t picID)
{
  char cmd[24] = "pic ";
  
  utoa(x, &cmd[strlen(cmd)], 10);
  strcat(cmd, ",");
  utoa(y, &cmd[strlen(cmd)], 10);
  strcat(cmd, ",");
  utoa(picID, &cmd[strlen(cmd)], 10);
  sendCommand(cmd);
}

void cropPic(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t picID)
{
  char cmd[32] = "picq ";
  
  utoa(x, &cmd[strlen(cmd)], 10);
  strcat(cmd, ",");
  utoa(y, &cmd[strlen(cmd)], 10);
  strcat(cmd, ",");
  utoa(w, &cmd[strlen(cmd)], 10);
  strcat(cmd, ",");
  utoa(h, &cmd[strlen(cmd)], 10);
  strcat(cmd, ",");
  utoa(picID, &cmd[strlen(cmd)], 10);
  sendCommand(cmd);
}

void drawText(uint32_t x, uint32_t y, uint32_t w, uint32_t h, NexTEXTALIGN_t centerX, NexTEXTALIGN_t centerY,
  uint32_t fontID, uint32_t fontColor, uint32_t backColor, NexBACKGROUND_t backStyle,
  const char* text)
{
  char cmd[48 + strlen(text)];
  
  memset(cmd, 0, sizeof(cmd));
  strcpy(cmd, "xstr ");
  utoa(x, &cmd[strlen(cmd)], 10);
  strcat(cmd, ",");
  utoa(y, &cmd[strlen(cmd)], 10);
  strcat(cmd, ",");
  utoa(w, &cmd[strlen(cmd)], 10);
  strcat(cmd, ",");
  utoa(h, &cmd[strlen(cmd)], 10);
  strcat(cmd, ",");
  utoa(fontID, &cmd[strlen(cmd)], 10);
  strcat(cmd, ",");
  utoa(fontColor, &cmd[strlen(cmd)], 10);
  strcat(cmd, ",");
  utoa(backColor, &cmd[strlen(cmd)], 10);
  strcat(cmd, ",");
  utoa(centerX, &cmd[strlen(cmd)], 10);
  strcat(cmd, ",");
  utoa(centerY, &cmd[strlen(cmd)], 10);
  strcat(cmd, ",");
  utoa(backStyle, &cmd[strlen(cmd)], 10);
  //strcat(cmd, ",\"");
  strcat(cmd, ",");
  strcat(cmd, text);
  //strcat(cmd, "\"");
  sendCommand(cmd);
}
void drawTextAbs(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, NexTEXTALIGN_t centerX, NexTEXTALIGN_t centerY,
  uint32_t fontID, uint32_t fontColor, uint32_t backColor, NexBACKGROUND_t backStyle,
  const char* text)
{
  if (x1 > x2) swap(&x1, &x2);
  if (y1 > y2) swap(&y1, &y2);
  drawText(x1, y1, x2 - x1, y2 - y1, centerX, centerY, fontID, fontColor, backColor, backStyle, text);
}


/**
* @file NexText.cpp
*
* The implementation of class NexText. 
*
* @author  Wu Pengfei (email:<pengfei.wu@itead.cc>)
* @date    2015/8/13
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

#include "NexText.h"

NexText::NexText(uint8_t pid, uint8_t cid, const char *name, void *value)
    :NexTouch(pid, cid, name, value)
{
}

uint16_t NexText::getText(char *buffer, uint16_t len)
{
  return NexObject::getString(NexTEXT, buffer, len);
  //char cmd[32] = "get ";
  //strcat(cmd, getObjName());
  //strcat(cmd, ".txt");
  //sendCommand(cmd);
  //return recvRetString(text, len);
}

bool NexText::setText(const char *buffer)
{
  return NexObject::setString(NexTEXT, buffer);
  //char cmd[32];
  //strcpy(cmd, getObjName());
  //strcat(cmd, ".txt=\"");
  //strcat(cmd, text);
  //strcat(cmd, "\"");
  //sendCommand(cmd);
  //return recvRetCommandFinished();
}

bool NexText::setForeColor(uint32_t number)
{
  return NexObject::setValue("pco", number);
}

bool NexText::setBackColor(uint32_t number)
{
  return NexObject::setValue("bco", number);
}



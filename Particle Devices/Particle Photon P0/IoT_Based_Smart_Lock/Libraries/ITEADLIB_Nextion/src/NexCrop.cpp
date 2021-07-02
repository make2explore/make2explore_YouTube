/**
* @file NexCrop.cpp
*
* The implementation of class NexCrop. 
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

#include "NexCrop.h"

NexCrop::NexCrop(uint8_t pid, uint8_t cid, const char *name, void *value)
    :NexTouch(pid, cid, name, value)
{
}

bool NexCrop::getPic(uint32_t *number)
{
  return NexObject::getValue("picc", number);
  //char cmd[32] = "get ";
  //strcat(cmd, getObjName());
  //strcat(cmd, ".picc");
  //sendCommand(cmd);
  //return recvRetNumber(number);
}

bool NexCrop::setPic(uint32_t number)
{
  return NexObject::setValue("picc", number);
  //char cmd[32];
  //strcpy(cmd, getObjName());
  //strcat(cmd, ".picc=");
  //utoa(number, &cmd[strlen(cmd)], 10);
  //sendCommand(cmd);
  //return recvRetCommandFinished();
}


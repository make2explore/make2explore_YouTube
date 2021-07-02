/**
* @file NexVar.cpp
*
* The implementation of class NexVar. 
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
* by ScruffR (Dec. 2015)
*/

#include "NexVar.h"

NexVar::NexVar(uint8_t pid, uint8_t cid, const char *name)
    :NexTouch(pid, cid, name)
{
}

uint16_t NexVar::getText(char *buffer, uint16_t len)
{
  return NexObject::getString(NexTEXT, buffer, len);
}

bool NexVar::setText(const char *buffer)
{
  return NexObject::setString(NexTEXT, buffer);
}

bool NexVar::getValue(uint32_t *number)
{
  return NexObject::getValue(NexVALUE, number);
}

bool NexVar::setValue(uint32_t number)
{
  return NexObject::setValue(NexVALUE, number);
}

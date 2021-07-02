/**
* @file NexNumber.cpp
*
* The implementation of class NexNumber. 
*
* @author  huang xianming (email:<xianming.huang@itead.cc>)
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

#include "NexNumber.h"

NexNumber::NexNumber(uint8_t pid, uint8_t cid, const char *name)
    :NexTouch(pid, cid, name)
{
}

bool NexNumber::getValue(uint32_t *number)
{
  return NexObject::getValue("val", number);
}

bool NexNumber::setValue(uint32_t number)
{
  return NexObject::setValue("val", number);
}


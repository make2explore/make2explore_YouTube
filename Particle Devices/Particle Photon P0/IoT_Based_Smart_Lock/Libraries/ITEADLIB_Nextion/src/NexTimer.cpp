/**
* @file NexTimer.cpp
*
* The implementation of class NexTimer. 
*
* @author  huang xianming (email:<xianming.huang@itead.cc>)
* @date    2015/8/26
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

#include "NexTimer.h"

NexTimer::NexTimer(uint8_t pid, uint8_t cid, const char *name, void *value)
    :NexTouch(pid, cid, name, value)
{
}

void NexTimer::attachTimer(NexTouchEventCb timer, void *ptr)
{
    NexTouch::attachPop(timer, ptr);
}

void NexTimer::detachTimer(void)
{
    NexTouch::detachPop();
}

bool NexTimer::getCycle(uint32_t *number)
{
  return NexObject::getValue("tim", number);
}

bool NexTimer::setCycle(uint32_t number)
{
  if (number < 50) number = 50;
  return NexObject::setValue("tim", number);
}

bool NexTimer::enable(void)
{
  return NexObject::setValue("en", 1);
}

bool NexTimer::disable(void)
{
  return NexObject::setValue("en", 0);
}

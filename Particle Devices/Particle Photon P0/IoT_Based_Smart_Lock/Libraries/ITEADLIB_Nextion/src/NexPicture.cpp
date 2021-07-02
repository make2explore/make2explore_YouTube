/**
* @file NexPicture.cpp
*
* The implementation of class NexPicture. 
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

#include "NexPicture.h"

NexPicture::NexPicture(uint8_t pid, uint8_t cid, const char *name, void *value)
    :NexTouch(pid, cid, name, value)
{
}

bool NexPicture::getPic(uint32_t *number)
{
  return NexObject::getValue("pic", number);
}

bool NexPicture::setPic(uint32_t number)
{
  return NexObject::setValue("pic", number);
}
 
/**
* @file Nextion.h
*
* The header file including all other header files provided by this library. 
*
* Every example sketch should include this file. 
*
* @author  Wu Pengfei (email:<pengfei.wu@itead.cc>)
* @date    2015/8/12
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

#ifndef __NEXTION_H__
#define __NEXTION_H__

#if defined(SPARK)
#include "application.h"
#else
#include "Arduino.h"
#endif

#include "NexConfig.h"
#include "NexCommands.h"
#include "NexHardware.h"

#include "NexTouch.h"
#include "NexButton.h"
#include "NexCrop.h"
#include "NexDualStateButton.h"
#include "NexGauge.h"
#include "NexHotspot.h"
#include "NexNumber.h"
#include "NexPage.h"
#include "NexPicture.h"
#include "NexProgressBar.h"
#include "NexSlider.h"
#include "NexText.h"
#include "NexVar.h"
#include "NexWaveform.h"
#include "NexTimer.h"

#include "NexGraphing.h"

#endif /* #ifndef __NEXTION_H__ */

/**
* @file NexConfig.h
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

#ifndef __NEXCONFIG_H__
#define __NEXCONFIG_H__

/**
 * @addtogroup Configuration 
 * @{ 
 */

 /**
 * Define the component fields to manipulate 
 * Matrix which field can be set on what component
 */

 // applies to:
#define NexBACKCOLOR    "bco"    // Text, Number, Button, Progressbar,        ,     , Gauge, Waveform, Slider,      ,         ,
#define NexBACKCOLOR0   "bco0"   //     ,       ,       ,            ,        ,     ,      ,         ,       ,      ,         , DualStateButton
#define NexBACKCOLOR1   "bco1"   //     ,       ,       ,            ,        ,     ,      ,         ,       ,      ,         , DualStateButton
#define NexBACKCOLORACT "bco2"   //     ,       , Button,            ,        ,     ,      ,         ,       ,      ,         ,
#define NexFORECOLOR    "pco"    // Text, Number, Button, Progressbar,        ,     , Gauge,         , Slider,      ,         ,
#define NexFORECOLOR0   "pco0"   //     ,       ,       ,            ,        ,     ,      , Waveform,       ,      ,         ,
#define NexFORECOLOR1   "pco1"   //     ,       ,       ,            ,        ,     ,      , Waveform,       ,      ,         ,
#define NexFORECOLOR2   "pco2"   //     ,       ,       ,            ,        ,     ,      , Waveform,       ,      ,         ,
#define NexFORECOLOR3   "pco3"   //     ,       ,       ,            ,        ,     ,      , Waveform,       ,      ,         ,
#define NexFORECOLORACT "pco2"   //     ,       , Button,            ,        ,     ,      ,         ,       ,      ,         ,
#define NexPICTURE      "pic"    // Text, Number, Button,            , Picture,     ,      , Waveform, Slider,      ,         ,
#define NexPICTURE0     "pic0"   //     ,       ,       ,            ,        ,     ,      ,         ,       ,      ,         , DualStateButton
#define NexPICTURE1     "pic1"   //     ,       ,       ,            ,        ,     ,      ,         ,       ,      ,         , DualStateButton
#define NexPICTUREACT   "pic2"   // Text, Number, Button,            ,        ,     ,      ,         , Slider,      ,         ,
#define NexCROPIMAGE    "picc"   // Text, Number, Button,            ,        , Crop, Gauge, Waveform, Slider,      ,         ,
#define NexCROPIMAGE0   "picc0"  //     ,       ,       ,            ,        ,     ,      ,         ,       ,      ,         , DualStateButton
#define NexCROPIMAGE1   "picc1"  //     ,       ,       ,            ,        ,     ,      ,         ,       ,      ,         , DualStateButton
#define NexCROPIMAGEACT "picc2"  // Text, Number, Button,            ,        ,     ,      ,         ,       ,      ,         ,
#define NexBACKPICTURE  "bpic"   //     ,       ,       , Progressbar,        ,     ,      ,         ,       ,      ,         ,
#define NexFOREPICTURE  "ppic"   //     ,       ,       , Progressbar,        ,     ,      ,         ,       ,      ,         ,
#define NexFONT         "font"   // Text, Number, Button,            ,        ,     ,      ,         ,       ,      ,         ,
#define NexCENTERX      "xcen"   // Text, Number, Button,            ,        ,     ,      ,         ,       ,      ,         ,
#define NexCENTERY      "ycen"   // Text, Number, Button,            ,        ,     ,      ,         ,       ,      ,         ,
#define NexTEXT         "txt"    // Text,       , Button,            ,        ,     ,      ,         ,       ,      , Variable,
#define NexVALUE        "val"    //     , Number,       , Progressbar,        ,     , Gauge,         , Slider,      , Variable, DualStateButton
#define NexMINVALUE     "minval" //     ,       ,       ,            ,        ,     ,      ,         , Slider,      ,         , 
#define NexMAXVALUE     "maxval" //     ,       ,       ,            ,        ,     ,      ,         , Slider,      ,         , 
#define NexLENGTH       "lenth"  //     , Number,       ,            ,        ,     ,      ,         ,       ,      ,         , 
#define NexWIDTH        "wid"    //     ,       ,       ,            ,        ,     , Gauge,         ,       ,      ,         , 
#define NexGRIDCOLOR    "gdc"    //     ,       ,       ,            ,        ,     ,      , Waveform,       ,      ,         , 
#define NexGRIDWIDTH    "gdw"    //     ,       ,       ,            ,        ,     ,      , Waveform,       ,      ,         , 
#define NexTHUMBWIDTH   "wid"    //     ,       ,       ,            ,        ,     ,      ,         , Slider,      ,         , 
#define NexTHUMBHEIGHT  "hig"    //     ,       ,       ,            ,        ,     ,      ,         , Slider,      ,         , 
#define NexINTERVAL     "tim"    //     ,       ,       ,            ,        ,     ,      ,         ,       , Timer,         , 
#define NexENABLED      "en"     //     ,       ,       ,            ,        ,     ,      ,         ,       , Timer,         , 

/** 
 * Define DEBUG_SERIAL_ENABLE to enable debug serial. 
 * Comment it to disable debug serial. 
 */
#define DEBUG_SERIAL_ENABLE  // include this in your sketch to activate debug output

/**
 * Define dbSerial for the output of debug messages. 
 */
#define dbSerial Serial

/**
 * Define nexSerial for communicate with Nextion touch panel. 
 * this has to be set in the main project file
 */
extern USARTSerial& nexSerial;


#ifdef DEBUG_SERIAL_ENABLE
#define dbSerialPrint(a)    dbSerial.print(a)
#define dbSerialPrintln(a)  dbSerial.println(a)
#define dbSerialBegin(a)    dbSerial.begin(a)
#else
#define dbSerialPrint(a)    ; // used to be do {} while(0)
#define dbSerialPrintln(a)  ; // used to be do {} while(0)
#define dbSerialBegin(a)    ; // used to be do {} while(0)
#endif

extern int bkCmd;             // command execution response behaviour (default 1 onSuccess)

/**
 * @}
 */

#endif /* #ifndef __NEXCONFIG_H__ */

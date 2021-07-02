/**
* @file NexCommands.h
*
* The definition of miscellaneous base API commands of Nextion.
* based on http://wiki.iteadstudio.com/Nextion_Instruction_Set
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

#ifndef __NEXCOMMANDS_H__
#define __NEXCOMMANDS_H__
#if defined(SPARK)
#include "application.h"
#else
#include "Arduino.h"
#endif
#include "NexConfig.h"

// Final %s in each format string can take the command terminator if required
//   it can be set as "" for sendCommand() or "\xff\xff\xff" for sendBatch()
#define NexCMDTERM    "\xff\xff\xff"
#define NexREFRESH    "ref %s%s"        // refresh component (can be number or name)
#define NexGETPAGE    "sendme%s"        // instructs the display to return the current page ID via serial
#define NexCONVERT    "cov %s,%s,%d%s"  // converts internal strings to value and vice versa
                                        //   third parameter describes string length 
                                        //   =0 auto, >0 fixed length
#define NexSTOPTOUCH  "cle_c%s"         // cancels all touch areas on the current page till page reload
#define NexCTLTOUCH   "tsw %s,%d%s"     // start(1)/stop(0) touch recognition on component
                                        //   tsw <component ID/ref>,1/0 
                                        //   255 as ref denotes all components of current page
#define NexCOMPAUSE   "com_stop%s"      // pauses execution of serial commands, but carries on receiving
                                        //   keep in mind the limited buffer size (see HW manual)
#define NexCOMRESUME  "com_star%s"      // resumes execution of serial commands including buffered ones
#define NexCOMCLEAR   "code_c%s"        // clears the commands buffer
#define NexPRINT      "print %s%s"      // instructs the display to send back some data (text or binary)
                                        //   e.g. print v0.val returns the four byte binary value
                                        //        print t0.txt returns the text
                                        //        print 123    returns binary value
                                        //        print "123"  returns 123
#define NexPRINTHEX   "printh %s%s"     // instructs the display to send back binary data queued as 
                                        //   hex string "FF FF FF ..." (exactly one blank as seperator)
#define NexTSPACEX    "spax=%d%s"       // set horizontal font spacing (default 0)
#define NexTSPACEY    "spay=%d%s"       // set vertical font spacing (default 0)
#define NexDRAWCOLOR  "thc=%d%s"        // set touch drawing color
#define NexDRAW       "thdra=%d%s"      // activate(1)/deactivate(0) touch drawing
#define NexSLEEPCOM   "ussp=%d%s"       // enter sleep after x seconds no serial communication (default 0=never)
#define NexSLEEPTOUCH "thsp=%d%s"       // enter sleep after x seconds no touch event (default 0=never)
#define NexWAKETOUCH  "thup=%d%s"       // wake on touch event (yes(1)/no(0))
#define NexSENDTOUCH  "sendxy=%d%s"     // activate(1)/deactivate(0) transmission of touch coordinates
#define NexDELAY      "delay=%d%s"      // pause code execution for x milli seconds
#define NexSLEEP      "sleep=%d%s"      // enter(1)/exit(0) sleep mode
#define NexEXRESPONSE "bkcmd=%d%s"      // set execution response type (0..3)
                                        //   0 .. none
                                        //   1 .. on success
                                        //   2 .. on fail
                                        //   3 .. always
#define NexCALIBRATE  "touch_j%s"       // triggers touch calibration (usually not needed)

int NexSendCommand(const char *cmdPattern, ...);

#endif

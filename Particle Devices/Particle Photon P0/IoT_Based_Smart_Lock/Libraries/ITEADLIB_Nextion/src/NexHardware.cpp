/**
* @file NexHardware.cpp
*
* The implementation of base API for using Nextion.
*
* @author  Wu Pengfei (email:<pengfei.wu@itead.cc>)
* @date    2015/8/11
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

#include "NexHardware.h"

#define NEX_RET_CMD_FINISHED                (0x01)
#define NEX_RET_EVENT_LAUNCHED              (0x88)
#define NEX_RET_EVENT_UPGRADED              (0x89)
#define NEX_RET_EVENT_TOUCH_HEAD            (0x65)     
#define NEX_RET_EVENT_POSITION_HEAD         (0x67)
#define NEX_RET_EVENT_SLEEP_POSITION_HEAD   (0x68)
#define NEX_RET_CURRENT_PAGE_ID_HEAD        (0x66)
#define NEX_RET_STRING_HEAD                 (0x70)
#define NEX_RET_NUMBER_HEAD                 (0x71)
#define NEX_RET_VALUE_HEAD                  (0x72)
#define NEX_RET_INVALID_CMD                 (0x00)
#define NEX_RET_INVALID_COMPONENT_ID        (0x02)
#define NEX_RET_INVALID_PAGE_ID             (0x03)
#define NEX_RET_INVALID_PICTURE_ID          (0x04)
#define NEX_RET_INVALID_FONT_ID             (0x05)
#define NEX_RET_INVALID_BAUD                (0x11)
#define NEX_RET_INVALID_VARIABLE            (0x1A)
#define NEX_RET_INVALID_OPERATION           (0x1B)


int bkCmd = 1;             // command execution response behaviour 
                           // 0 .. no response
                           // 1 .. onSuccess (default
                           // 2 .. noFail
                           // 3 .. always

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
bool recvRetNumber(uint32_t *number, uint32_t timeout)
{
  bool ret = false;
  uint8_t temp[8] = { 0 };

  if (!number)
  {
    goto __return;
  }

  nexSerial.setTimeout(timeout);
  if (sizeof(temp) != nexSerial.readBytes((char *)temp, sizeof(temp)))
  {
    goto __return;
  }

  if (temp[0] == NEX_RET_NUMBER_HEAD
    && temp[5] == 0xFF
    && temp[6] == 0xFF
    && temp[7] == 0xFF
    )
  {
    *number = (temp[4] << 24) | (temp[3] << 16) | (temp[2] << 8) | (temp[1]);
    ret = true;
  }

__return:

  if (ret)
  {
    dbSerialPrint("recvRetNumber :");
    dbSerialPrintln(*number);
  }
  else
  {
    dbSerialPrintln("recvRetNumber err");
  }

  return ret;
}


/*
 * Receive string data.
 *
 * @param buffer - save string data.
 * @param len - string buffer length.
 * @param timeout - set timeout time.
 *
 * @return the length of string buffer.
 *
 */
uint16_t recvRetString(char *buffer, uint16_t len, uint32_t timeout)
{
  uint16_t ret = 0;
  bool str_start_flag = false;
  uint8_t cnt_0xff = 0;
  uint8_t c = 0;
  uint32_t start;

  uint16_t bufIdx = 0;
  //uint16_t tempIdx = 0;
  //char temp[len];
  //memset(temp, 0, len);

  if (!buffer || len == 0)
  {
    goto __return;
  }
  memset(buffer, 0, len);

  start = millis();
  while (cnt_0xff < 3 && millis() - start <= timeout)
  {
#if defined(SPARK)
    Particle.process();
#endif
    while (nexSerial.available())
    {
#if defined(SPARK)
      Particle.process();
#endif
      c = nexSerial.read();
      if (str_start_flag)
      {
        if (0xFF == c)
        {
          cnt_0xff++;
          if (cnt_0xff >= 3)
          {
            break;
          }
        }
        else if ((int)bufIdx < len - 1)
        {
          buffer[bufIdx++] = (char)c;
          //temp[tempIdx++] = (char)c;
          //temp[tempIdx] = '\0';  // memset makes sure of this already
        }
      }
      else if (NEX_RET_STRING_HEAD == c)
      {
        str_start_flag = true;
      }
    }
  }

  ret = bufIdx;
  //ret = strlen(temp);
  //ret = ret > len ? len : ret;
  //strncpy(buffer, temp, ret);

__return:

  dbSerialPrint("recvRetString[");
  dbSerialPrint(strlen(buffer));
  dbSerialPrint(",");
  dbSerialPrint(buffer);
  dbSerialPrintln("]");

  return ret;
}

/*
 * Send command to Nextion.
 *
 * @param cmd - the string of command.
 */
void sendCommand(const char* cmd)
{
  while (nexSerial.read() >= 0);  // flush RX buffer only
  nexSerial.print(cmd);
  nexSerial.print(NexCMDTERM);
  //nexSerial.write(0xFF);
  //nexSerial.write(0xFF);
  //nexSerial.write(0xFF);
}

/*
* Send a batch of commands (skript) each terminated by three 0xFF 
* bytes to Nextion.
* If noRR is set TRUE
*   For the batch the command execution result response gets 
*   temporarily deactivated.
*   After completion it will be reactivated and set to the 
*   previously saved state (bkCmd).
*
* @param cmd  - the string of command.
* @param noRR - indicate if result response should be deactivated (default false)
*/
void sendSkript(const char* cmd, bool noRR)
{
  char buf[12];
  while (nexSerial.read() >= 0);            // flush RX buffer only
  if (noRR)
    nexSerial.print("bkcmd=0\xff\xff\xff"); // deactivate result response
  nexSerial.print(cmd);                 
  if (noRR)
  {
    snprintf(buf, sizeof(buf), "bkcmd=%d\xff\xff\xff", bkCmd);
    nexSerial.print(buf);                     // reactivate previous command response
  }
}

/*
 * Command is executed successfully.
 *
 * @param timeout - set timeout time.
 *
 * @retval true - success.
 * @retval false - failed.
 *
 */
bool recvRetCommandFinished(uint32_t timeout)
{
  bool ret = false;
  uint8_t temp[4] = "";

  nexSerial.setTimeout(timeout);
  if (sizeof(temp) != nexSerial.readBytes((char *)temp, sizeof(temp)))
  {
    ret = false;
  }

  if (temp[0] == NEX_RET_CMD_FINISHED
    && temp[1] == 0xFF
    && temp[2] == 0xFF
    && temp[3] == 0xFF
    )
  {
    ret = true;
  }

  if (ret)
  {
    dbSerialPrintln("recvRetCommandFinished ok");
  }
  else
  {
    dbSerialPrintln("recvRetCommandFinished err");
  }

  return ret;
}


bool nexInit(uint32_t baudrate)
{
  bool ret1 = false;
  bool ret2 = false;

  nexSerial.begin(baudrate);

  sendCommand("");
  sendCommand("bkcmd=1");  // only return success results
  ret1 = recvRetCommandFinished();
  sendCommand("page 0");   // show home page
  ret2 = recvRetCommandFinished();
  return ret1 && ret2;
}

void nexLoop(NexTouch *nex_listen_list[])
{
  static uint8_t __buffer[20];

  uint16_t i;
  uint8_t c;

  while (nexSerial.available())
  {
#if defined(SPARK)
    Particle.process();
#endif
    delay(10); 

    c = nexSerial.read();

    if (NEX_RET_EVENT_TOUCH_HEAD == c)
    {
      if (nexSerial.available() >= 6)
      {
        __buffer[0] = c;
        for (i = 1; i < 7; i++)
        {
          __buffer[i] = nexSerial.read();
        }
        __buffer[i] = 0x00;

        if (0xFF == __buffer[4] && 0xFF == __buffer[5] && 0xFF == __buffer[6])
        {
          NexTouch::iterate(nex_listen_list, __buffer[1], __buffer[2], (int32_t)__buffer[3], NULL);
        }

      }
    }
    else if (NEX_RET_CURRENT_PAGE_ID_HEAD == c)
    {
      dbSerialPrint("page cmd:");
      if (nexSerial.available() >= 4)
      {
        __buffer[0] = c;
        for (i = 1; i < 5; i++)
        {
          __buffer[i] = nexSerial.read();
        }
        __buffer[i] = 0x00;

        if (0xFF == __buffer[2] && 0xFF == __buffer[3] && 0xFF == __buffer[4])
        {
          dbSerialPrintln(__buffer[1]);
          NexTouch::iterate(nex_listen_list, __buffer[1], 0, (int32_t)NEX_EVENT_PUSH, NULL);
        }

      }
    }
    else if (NEX_RET_VALUE_HEAD == c)
    {
      dbSerialPrint("value cmd:");
      if (nexSerial.available() >= 10)
      {
        __buffer[0] = c;
        for (i = 1; i < 12; i++)
        {
          __buffer[i] = nexSerial.read();
        }
        __buffer[i] = 0x00;

        if (0xFF == __buffer[i - 1] && 0xFF == __buffer[i - 2] && 0xFF == __buffer[i - 3])
        {
          dbSerialPrint(" Page:");
          dbSerialPrint(__buffer[1]);
          dbSerialPrint(" Component:");
          dbSerialPrint(__buffer[2]);
          dbSerialPrint(" Value:");
          i = __buffer[4] | (((unsigned long)__buffer[5]) << 8) | (((unsigned long)__buffer[6]) << 16) | (((unsigned long)__buffer[6]) << 24);
          dbSerialPrintln(i);
          NexTouch::iterate(nex_listen_list, __buffer[1], __buffer[2], (int32_t)__buffer[3], (void *)&(__buffer[4]));
        }
      }
    }
  }
}

/**
 * Return current page id.
 *
 * @param pageId - output parameter,to save page id.
 *
 * @retval true - success.
 * @retval false - failed.
 */
bool sendCurrentPageId(uint8_t* pageId)
{

  bool ret = false;
  uint8_t temp[5] = { 0 };

  if (!pageId)
  {
    goto __return;
  }
  sendCommand("sendme");
  delay(50);
  nexSerial.setTimeout(100);
  if (sizeof(temp) != nexSerial.readBytes((char *)temp, sizeof(temp)))
  {
    goto __return;
  }

  if (temp[0] == NEX_RET_CURRENT_PAGE_ID_HEAD
    && temp[2] == 0xFF
    && temp[3] == 0xFF
    && temp[4] == 0xFF
    )
  {
    *pageId = temp[1];
    ret = true;
  }

__return:

  if (ret)
  {
    dbSerialPrint("recvPageId :");
    dbSerialPrintln(*pageId);
  }
  else
  {
    dbSerialPrintln("recvPageId err");
  }

  return ret;

}

/**
 * Set current backlight brightness value.
 *
 * @param dimValue - current backlight brightness value.
 *
 * @retval true - success.
 * @retval false - failed.
 */
bool setCurrentBrightness(uint8_t dimValue, bool setDefault)
{
  bool ret = false;
  char cmd[16] = "dim=";

  if (setDefault)
    strcpy(cmd, "dims=");

  utoa(dimValue, &cmd[strlen(cmd)], 10);
  sendCommand(cmd);
  delay(10);

  if (recvRetCommandFinished())
  {
    dbSerialPrint("setCurrentBrightness[ ");
    dbSerialPrint(dimValue);
    dbSerialPrintln("]ok ");

    ret = true;
  }
  else
  {
    dbSerialPrintln("setCurrentBrightness err ");
  }

  return ret;
}

/**
 * Set default baudrate.
 *
 * @param  defaultBaudrate - default baudrate,it supports 2400,4800,9600,19200,38400,57600,115200.
 *
 * @retval true - success.
 * @retval false - failed.
 */
bool setDefaultBaudrate(uint32_t defaultBaudrate)
{
  bool ret = false;
  char cmd[16] = "bauds=";

  utoa(defaultBaudrate, &cmd[strlen(cmd)], 10);
  sendCommand(cmd);
  delay(10);

  if (recvRetCommandFinished())
  {
    dbSerialPrintln("setDefaultBaudrate ok ");
    ret = true;
  }
  else
  {
    dbSerialPrintln("setDefaultBaudrate err ");
  }

  nexSerial.begin(defaultBaudrate);

  return ret;
}


/**
* Set current baudrate.
*
* @param  baudrate - baudrate, it supports 2400,4800,9600,19200,38400,57600,115200.
*
* @retval true - success.
* @retval false - failed.
*/
bool setBaudrate(uint32_t baudrate)
{
  bool ret = false;
  char cmd[16] = "baud=";

  utoa(baudrate, &cmd[strlen(cmd)], 10);
  sendCommand(cmd);           // send in new baudrate using the current baudrate
  delay(10);

  nexSerial.flush();          // dump all returned data, since not usable with new baudrate
  nexSerial.end();            // close port (or pretend to ;-)
  nexSerial.begin(baudrate);  // activate new baudrate on MCU side too
  sendCommand("");            // trigger test transmission

  if (recvRetCommandFinished())
  {
    dbSerialPrintln("setBaudrate ok ");
    ret = true;
  }
  else
  {
    dbSerialPrintln("setBaudrate err ");
  }

  return ret;
}


void sendRefreshAll(void)
{
  sendCommand("ref 0");
}


bool NexGetValue(const char* objName, const char* valueType, uint32_t* value)
{
  //char cmd[32];
  //snprintf(cmd, sizeof(cmd), "get %s.%s", objName, valueType);
  char cmd[32] = "get ";
  strcat(cmd, objName);
  strcat(cmd, ".");
  strcat(cmd, valueType);
  sendCommand(cmd);
  return recvRetNumber(value);
}

bool NexSetValue(const char* objName, const char* valueType, uint32_t value)
{
  char cmd[32];
  //snprintf(cmd, sizeof(cmd), "%s.%s=%d", objName, valueType, value);
  strcpy(cmd, objName);
  strcat(cmd, ".");
  strcat(cmd, valueType);
  strcat(cmd, "=");
  utoa(value, &cmd[strlen(cmd)], 10);
  sendCommand(cmd);
  return recvRetCommandFinished();
}

uint16_t NexGetString(const char* objName, const char* valueType, char* text, uint16_t len)
{
  //char cmd[32];
  //snprintf(cmd, sizeof(cmd), "get %s.%s", objName, valueType);
  char cmd[32] = "get ";
  strcat(cmd, objName);
  strcat(cmd, ".");
  strcat(cmd, valueType);
  sendCommand(cmd);
  return recvRetString(text, len);
}

bool NexSetString(const char* objName, const char* valueType, const char* text)
{
  char cmd[strlen(text) + 32];
  memset(cmd, 0, sizeof(cmd));
  //snprintf(cmd, sizeof(cmd), "%s.%s=\"%s\"", objName, valueType, text);
  strcpy(cmd, objName);
  strcat(cmd, ".");
  strcat(cmd, valueType);
  strcat(cmd, "=\"");
  strcat(cmd, text);
  strcat(cmd, "\"");
  sendCommand(cmd);
  return recvRetCommandFinished();
}

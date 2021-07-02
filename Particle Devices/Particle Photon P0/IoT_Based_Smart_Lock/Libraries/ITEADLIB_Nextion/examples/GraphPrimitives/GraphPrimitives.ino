
/**
 * @example GraphPrimitives.ino
 *
 * @par How to Use
 * Show how to use API for drawing primitives
 *
 * @author  Andreas Rothenw�nder (aka ScruffR <http://community.particle.io>)
 * @date    2015/12/06
 * @copyright 
 * Copyright (C) 2014-2015 ScruffR \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */ 

#include "Nextion.h"
USARTSerial& nexSerial = Serial1;

void setup(void)
{
    nexInit();
    dbSerialPrintln("setup done");
    setBaudrate(115200);

    clearScreen(0x0000);
    drawPic(0, 0, 0);
}

int x;
int y;
void loop(void)
{
    cropPic(320-x, y + 25, 50, 50, 1);
    x += 4;
    x %= 320;
    y += 3;
    y %= 215;
    drawText(0, 0, 320, 25, NexCENTER, NexCENTER, 0, 0xFFFF, 0xAAAA, NexSOLID, "Particle rocks!");
    drawLine(5, 5, 50, 30, 0xF800);
    fillRect(250, 25, 50, 25, 0x7E00);
    drawRect(20, 105, 50, 25, 0x01FF);
    drawCircle(160, 120, 30, 0xF9FF);

    delay(25);
}

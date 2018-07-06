/*  AdaHomePage.cpp
 *
 *      Defines the behavior of our home page, which displays the thermostat.
 *  
 *  
 *  ArduinoThermostat copyright © 2018 by William Edward Woody
 *  
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the Free 
 *  Software Foundation, either version 3 of the License, or (at your option) 
 *  any later version.
 *  
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for 
 *  more details.
 *  
 *  You should have received a copy of the GNU General Public License with this
 *  source distribution. If you did not, you may download a copy of the GNU
 *  General Public License at http://www.gnu.org/licenses/
 *  
 *  If you wish for a commercial license which does not require the
 *  distribution of this source code, or if you wish support with modification
 *  of this code, please contact:
 *  
 *  William Edward Woody
 *  12605 Raven Ridge Rd
 *  Raleigh, NC 27614
 *  United States of America
 *  woody@alumni.caltech.edu
 *
 *
 *  The original sources for this Arduino application may be downloaded from
 *  https://github.com/w3woody/ArduinoThermostat
 */

#include "AdaUIScreen.h"
#include "AdaHomePage.h"
#include "Narrow25D.h"
#include "Narrow75D.h"
#include "AdaTime.h"
#include "AdaThermostat.h"
#include "AdaSettingsPage.h"
#include "AdaUtils.h"
#include "AdaSchedule.h"
#include "AdaProgmem.h"
#include "AdaStrings.h"

#include "AdaTempPage.h"
#include "AdaSchedulePickerPage.h"

/************************************************************************/
/*                                                                      */
/*  Layout Constants                                                    */
/*                                                                      */
/************************************************************************/

static const AdaUIRect AHomeRects[] PROGMEM = {
    { 160, 200, 40, 37 },       // Heat button
    { 201, 200, 40, 37 }        // Cool button
};

static const char* const AHomeTitles[] PROGMEM = {
    string_fan, string_schedule, NULL, NULL, string_settings
};

static const AdaPage AHome PROGMEM = {
    NULL, NULL, AHomeTitles, AHomeRects, 2
};

/************************************************************************/
/*                                                                      */
/*  Page Globals                                                        */
/*                                                                      */
/************************************************************************/

static AdaTempPage GTempPage;
static AdaSchedulePickerPage GSchedulePage;
static AdaSettingsPage GSettingsPage;

/************************************************************************/
/*                                                                      */
/*  Drawing Support                                                     */
/*                                                                      */
/************************************************************************/

const uint8_t Bullet_bitmap[] PROGMEM = {
    0x02, 0x85, 0x80,
    0x01, 0x87, 0x80,
    0x89, 0x80,
    0x89, 0x80,
    0x89, 0x80,
    0x89, 0x80,
    0x89, 0x80,
    0x01, 0x87, 0x80,
    0x02, 0x85, 0x00
};


// Bitmap for ring
const uint8_t Arc_bitmap[] PROGMEM = {
    0x80,0x44,0x99,0x80,0x3E,0xA4,0x80,0x3B,
    0xAB,0x80,0x37,0xB3,0x80,0x34,0xB9,0x80,
    0x32,0xBD,0x80,0x30,0xC1,0x80,0x2E,0xC5,
    0x80,0x2C,0xC9,0x80,0x2A,0xCE,0x80,0x28,
    0xA0,0x11,0xA0,0x80,0x26,0x9C,0x1D,0x9C,
    0x80,0x25,0x99,0x25,0x99,0x80,0x24,0x97,
    0x2B,0x98,0x80,0x22,0x96,0x31,0x96,0x80,
    0x20,0x95,0x37,0x94,0x80,0x1F,0x94,0x3B,
    0x94,0x80,0x1E,0x93,0x3F,0x93,0x80,0x1D,
    0x92,0x43,0x92,0x80,0x1B,0x92,0x47,0x91,
    0x80,0x1A,0x92,0x49,0x91,0x80,0x1A,0x90,
    0x4D,0x91,0x80,0x18,0x91,0x4F,0x91,0x80,
    0x17,0x91,0x52,0x90,0x80,0x16,0x90,0x55,
    0x8F,0x80,0x16,0x8F,0x57,0x8F,0x80,0x15,
    0x8F,0x59,0x90,0x80,0x14,0x8F,0x5B,0x8F,
    0x80,0x13,0x8E,0x5F,0x8E,0x80,0x12,0x8E,
    0x61,0x8E,0x80,0x11,0x8E,0x63,0x8E,0x80,
    0x11,0x8D,0x65,0x8E,0x80,0x10,0x8D,0x66,
    0x8E,0x80,0x0F,0x8D,0x68,0x8E,0x80,0x0E,
    0x8E,0x69,0x8E,0x80,0x0E,0x8D,0x6B,0x8D,
    0x80,0x0D,0x8D,0x6D,0x8D,0x80,0x0D,0x8C,
    0x6F,0x8D,0x80,0x0C,0x8C,0x70,0x8D,0x80,
    0x0B,0x8D,0x71,0x8D,0x80,0x0B,0x8C,0x73,
    0x8C,0x80,0x0A,0x8C,0x75,0x8C,0x80,0x0A,
    0x8C,0x75,0x8C,0x80,0x09,0x8C,0x77,0x8C,
    0x80,0x09,0x8B,0x79,0x8B,0x80,0x08,0x8C,
    0x79,0x8C,0x80,0x08,0x8B,0x7B,0x8B,0x80,
    0x07,0x8C,0x7B,0x8C,0x80,0x07,0x8B,0x7D,
    0x8B,0x80,0x06,0x8C,0x7D,0x8C,0x80,0x06,
    0x8B,0x7E,0x8C,0x80,0x06,0x8B,0x7F,0x8C,
    0x80,0x05,0x8B,0x7F,0x02,0x8B,0x80,0x05,
    0x8B,0x7F,0x02,0x8B,0x80,0x04,0x8C,0x7F,
    0x02,0x8C,0x80,0x04,0x8B,0x7F,0x04,0x8B,
    0x80,0x04,0x8B,0x7F,0x04,0x8B,0x80,0x04,
    0x8B,0x7F,0x04,0x8C,0x80,0x03,0x8B,0x7F,
    0x06,0x8B,0x80,0x03,0x8B,0x7F,0x06,0x8B,
    0x80,0x03,0x8B,0x7F,0x06,0x8B,0x80,0x03,
    0x8A,0x7F,0x08,0x8B,0x80,0x02,0x8B,0x7F,
    0x08,0x8B,0x80,0x02,0x8B,0x7F,0x08,0x8B,
    0x80,0x02,0x8B,0x7F,0x08,0x8B,0x80,0x02,
    0x8A,0x7F,0x0A,0x8A,0x80,0x02,0x8A,0x7F,
    0x0A,0x8A,0x80,0x01,0x8B,0x7F,0x0A,0x8B,
    0x80,0x01,0x8B,0x7F,0x0A,0x8B,0x80,0x01,
    0x8B,0x7F,0x0A,0x8B,0x80,0x01,0x8B,0x7F,
    0x0A,0x8B,0x80,0x01,0x8A,0x7F,0x0C,0x8A,
    0x80,0x01,0x8A,0x7F,0x0C,0x8A,0x80,0x01,
    0x8A,0x7F,0x0C,0x8A,0x80,0x01,0x8A,0x7F,
    0x0C,0x8A,0x80,0x01,0x8A,0x7F,0x0C,0x8A,
    0x80,0x01,0x8A,0x7F,0x0C,0x8A,0x80,0x01,
    0x8A,0x7F,0x0C,0x8A,0x80,0x01,0x8A,0x7F,
    0x0C,0x8A,0x80,0x01,0x8A,0x7F,0x0C,0x8A,
    0x80,0x01,0x8A,0x7F,0x0C,0x8A,0x80,0x01,
    0x8A,0x7F,0x0C,0x8A,0x80,0x01,0x8A,0x7F,
    0x0C,0x8A,0x80,0x01,0x8A,0x7F,0x0C,0x8A,
    0x80,0x01,0x8A,0x7F,0x0C,0x8A,0x80,0x01,
    0x8A,0x7F,0x0C,0x8A,0x80,0x01,0x8A,0x7F,
    0x0C,0x8A,0x80,0x01,0x8A,0x7F,0x0C,0x8A,
    0x80,0x01,0x8B,0x7F,0x0A,0x8B,0x80,0x01,
    0x8B,0x7F,0x0A,0x8B,0x80,0x01,0x8B,0x7F,
    0x0A,0x8B,0x80,0x01,0x8B,0x7F,0x0A,0x8B,
    0x80,0x02,0x8A,0x7F,0x0A,0x8A,0x80,0x02,
    0x8A,0x7F,0x0A,0x8A,0x80,0x02,0x8B,0x7F,
    0x08,0x8B,0x80,0x02,0x8B,0x7F,0x08,0x8B,
    0x80,0x02,0x8B,0x7F,0x08,0x8B,0x80,0x02,
    0x8B,0x7F,0x08,0x8A,0x80,0x03,0x8B,0x7F,
    0x06,0x8B,0x80,0x03,0x8B,0x7F,0x06,0x8B,
    0x80,0x03,0x8B,0x7F,0x06,0x8B,0x80,0x04,
    0x8B,0x7F,0x04,0x8C,0x80,0x04,0x8B,0x7F,
    0x04,0x8B,0x80,0x04,0x8B,0x7F,0x04,0x8B,
    0x80,0x05,0x8B,0x7F,0x02,0x8C,0x80,0x05,
    0x8B,0x7F,0x02,0x8B,0x80,0x05,0x8C,0x7F,
    0x01,0x8B,0x80,0x06,0x8B,0x7F,0x8C,0x80,
    0x06,0x8B,0x7F,0x8B,0x80,0x06,0x8C,0x7D,
    0x8C,0x80,0x07,0x8B,0x7D,0x8B,0x80,0x07,
    0x8C,0x7B,0x8C,0x80,0x08,0x8B,0x7B,0x8B,
    0x80,0x08,0x8C,0x79,0x8C,0x80,0x09,0x8B,
    0x79,0x8B,0x80,0x09,0x8C,0x77,0x8C,0x80,
    0x0A,0x8C,0x75,0x8C,0x80,0x0A,0x8C,0x75,
    0x8C,0x80,0x0B,0x8C,0x73,0x8C,0x80,0x0B,
    0x8D,0x71,0x8D,0x80,0x0C,0x8D,0x70,0x8C,
    0x80,0x0C,0x8D,0x6F,0x8D,0x80,0x0D,0x8D,
    0x6D,0x8D,0x80,0x0E,0x8D,0x6B,0x8D,0x80,
    0x0F,0x8D,0x69,0x8E,0x80,0x0F,0x8E,0x67,
    0x8E,0x80,0x10,0x8D,0x66,0x8E,0x80,0x10,
    0x8E,0x64,0x8F,0x80,0x11,0x8E,0x62,0x8F,
    0x80,0x12,0x8D,0x63,0x8D,0x80,0x13,0x8B,
    0x65,0x8B,0x80,0x14,0x89,0x67,0x8A,0x80,
    0x14,0x88,0x69,0x87,0x80,0x15,0x86,0x6B,
    0x85,0x80,0x17,0x83,0x6D,0x84,0x80,0x17,
    0x82,0x6F,0x82,0x00
};

const int8_t Arc_positions[] PROGMEM = {
    -60, 60,
    -66, 52,
    -72, 44,
    -77, 35,
    -80, 26,
    -83, 16,
    -84, 6,
    -84, -3,
    -83, -13,
    -81, -23,
    -78, -32,
    -74, -41,
    -68, -49,
    -62, -57,
    -55, -64,
    -47, -70,
    -38, -75,
    -29, -79,
    -19, -82,
    -9, -84,
    0, -85,
    9, -84,
    19, -82,
    29, -79,
    38, -75,
    47, -70,
    55, -64,
    62, -57,
    68, -49,
    74, -41,
    78, -32,
    81, -23,
    83, -13,
    84, -3,
    84, 6,
    83, 16,
    80, 26,
    77, 35,
    72, 44,
    66, 52,
    60, 60
};

void DrawRLEBitmap(const uint8_t *p, uint16_t color, int16_t x, int16_t y)
{
    int xc = x;
    int yc = y;
    uint8_t c;
    
    GC.startWrite();

    while (0 != (c = pgm_read_byte(p++))) {
        if (c == 0x80) {
            yc++;
            xc = x;
        } else if (c & 0x80) {
            // Horizontal band
            GC.writeFastHLine(xc,yc,0x7F & c,color);
        }
        xc += c & 0x7F;
    }
    
    GC.endWrite();
}

uint16_t AirConColor()
{
    uint16_t color;
    if (GThermostat.heat) {
        color = 0xF800;
    } else if (GThermostat.cool) {
        color = 0x001F;
    } else if (GThermostat.fan) {
        color = ADAUI_YELLOW;
    } else {
        color = 0x4208; // 0x4310;
    }
    return color;
}

void DrawArc(int16_t x, int16_t y)
{
    DrawRLEBitmap(Arc_bitmap,AirConColor(),x,y);
}

void DrawMarker(uint8_t temp, uint16_t color, bool outside)
{
    int16_t n = temp;
    if (n < 50) n = 50;
    if (n > 90) n = 90;
    n -= 50;
    
    int16_t xpos = (int8_t)pgm_read_byte(Arc_positions + n*2);
    int16_t ypos = (int8_t)pgm_read_byte(Arc_positions + n*2+1);
    if (!outside) {
        xpos = (xpos * 66)/85;
        ypos = (ypos * 66)/85;
    }
    xpos += 200;
    ypos += 150;
    
    DrawRLEBitmap(Bullet_bitmap,color,xpos-4,ypos-4);
    
    /*
     *  Bump text depending on position around Arc_bitmap
     */
     
    if (xpos < 200) {
        if (ypos < 150) {
            xpos -= 29;
            ypos -= 3;
        } else {
            xpos -= 29;
            ypos += 18;
        }
    } else {
        if (ypos < 150) {
            xpos += 7;
            ypos -= 3;
        } else {
            xpos += 7;
            ypos += 18;
        }
    }
    
    if (outside) {
        char buffer[8];
        FormatNumber(buffer,temp);
        GC.drawButton(RECT(xpos,ypos-19,22,24),buffer,19,0,KCenterAlign);
    }
}

void DrawTemperatureMarker(uint8_t temp, uint16_t color)
{
    DrawMarker(temp,color,true);
}

/************************************************************************/
/*                                                                      */
/*  Construction                                                        */
/*                                                                      */
/************************************************************************/

/*  AdaHomePage::AdaHomePage
 *
 *      Construct me
 */

AdaHomePage::AdaHomePage() : AdaUIPage(&AHome)
{
}

/************************************************************************/
/*                                                                      */
/*  Drawing Support                                                     */
/*                                                                      */
/************************************************************************/

/*  AdaHomePage::drawTitle
 *
 *      Draw the current title. This draws the time in the upper right
 *  corner of our display.
 */

void AdaHomePage::drawTitle()
{
    char buffer[16];

    // Note: our font and color was set in the caller
    FormatDayTime(buffer,AdaGetTime());
    GC.drawButton(RECT(160,0,160,32),buffer,24);
}

/*  AdaHomePage::drawContents
 *
 *      Draw the contents
 */

void AdaHomePage::drawContents()
{
    char buffer[8];

    /*
     *  Draw the thermostat settings
     */
    
    GC.setTextColor(ADAUI_PURPLE,ADAUI_BLACK);
    DrawTemperatureMarker(lastHeat,0xF800);       // red
    DrawTemperatureMarker(lastCool,0x001F);       // blue
    
    // Draw temperature arc
    DrawArc(120,70);

    // Draw temperature adjustment
    GC.setTextColor(ADAUI_BLUE,ADAUI_DARKGRAY);
    FormatNumber(buffer,GThermostat.heatSetting);
    GC.drawButton(RECT(160,200,40,37),buffer,28,KCornerUL | KCornerLL,KCenterAlign);
    FormatNumber(buffer,GThermostat.coolSetting);
    GC.drawButton(RECT(201,200,40,37),buffer,28,KCornerUR | KCornerLR,KCenterAlign);

    // Draw the temperature
    FormatNumber(buffer,GThermostat.curTemperature);
    GC.setTextColor(0xFFFF,ADAUI_BLACK);
    GC.setFont(&Narrow75D);

    // Draw type of schedule active and the temperature itself
    if (GThermostat.lastSet != 0xFF) {
        GC.drawButton(RECT(160,100,80,65),buffer,56,0,KCenterAlign);

        GC.setTextColor(ADAUI_PURPLE,ADAUI_BLACK);
        
        GC.setFont(&Narrow25D);
        GC.drawButton(RECT(160,160,80,30),GScheduleString(GThermostat.lastSet),26,0,KCenterAlign);
    } else {
        GC.drawButton(RECT(160,115,80,65),buffer,56,0,KCenterAlign);
    }
    
    // Draw the current temperature marker
    DrawMarker(GThermostat.curTemperature,0xFFFF,false);
    
    // This does violate our UI guidelines, but we have this nice
    // area in the upper left, and the color scheme makes this look
    // different from a back button. And sometimes violating our UI
    // guidelines is okay if what we're showing is sufficiently visually
    // distinct users won't get confused.
    //
    // In this case, the icons we're displaying don't look like a back button
    
    // Draw the heat/cool/fan icons in the lower right. We use the h/c/f characters
    // which are actually our icons. Part of this is for debugging purposes.
    char flags[4];
    char *ptr = flags;
    if (GThermostat.heat) *ptr++ = 'h';
    if (GThermostat.cool) *ptr++ = 'c';
    if (GThermostat.fan)  *ptr++ = 'f';
    *ptr = 0;
    GC.setTextColor(AirConColor(),ADAUI_BLACK);
    GC.drawButton(RECT(0,0,44,32),flags,24,0,KLeftAlign);
}

/*  AdaHomePage::handleEvent
 *
 *      Scan to see if the time changed, if the temperature changed, and what
 *  state the thermostat is inline. Also handle hit detection of my hand-drawn
 */

void AdaHomePage::handleEvent(uint8_t ix)
{
    switch (ix) {
        case AEVENT_LEFTBUTTON1:
        case AEVENT_FIRSTSPOT:
        case AEVENT_FIRSTSPOT+1:
            pushPage(&GTempPage);
            break;
        case AEVENT_LEFTBUTTON2:
            pushPage(&GSchedulePage);
            break;
        case AEVENT_LEFTBUTTON5:
            pushPage(&GSettingsPage);
            break;
    }
}

void AdaHomePage::periodicEvents()
{
    /*
     *  Check and redraw the time. Note we throttle to only redraw every
     *  minute rather than once a second.
     */
    
    uint8_t t = AdaGetTime() / 60;
    if (drewTime != t) {
        drewTime = t;

        GC.setTextColor(ADAUI_RED,ADAUI_BLACK);
        GC.setFont(&Narrow25D);
        drawTitle();
    }

    /*
     *  Determine the last state we draw. If it has changed, ask for a
     *  redraw. We just redraw the whole display, and yes I know this
     *  causes our world to flicker, but I assume this is a rare event.
     */
    
    if ((GThermostat.heatSetting != lastHeat) || (GThermostat.coolSetting != lastCool) ||
        (GThermostat.curTemperature != lastTemp) || (GThermostat.heat != lastHeatOn) || 
        (GThermostat.cool != lastCoolOn) || (GThermostat.fan != lastFanOn)) {
        
        lastHeat = GThermostat.heatSetting;
        lastCool = GThermostat.coolSetting;
        lastTemp = GThermostat.curTemperature;
        lastHeatOn = GThermostat.heat;
        lastCoolOn = GThermostat.cool;
        lastFanOn = GThermostat.fan;
        
        invalidateContents();
    }
}

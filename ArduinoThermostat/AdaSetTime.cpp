/*  AdaSetTime.cpp
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
#include "AdaSetTime.h"
#include "Narrow25D.h"
#include "AdaTime.h"
#include "AdaThermostat.h"
#include "AdaUtils.h"
#include "AdaProgmem.h"
#include "AdaStrings.h"

/************************************************************************/
/*                                                                      */
/*  Layout Constants                                                    */
/*                                                                      */
/************************************************************************/

static const AdaPage ATime PROGMEM = {
    string_set_time, string_back, NULL, NULL, 0
};

static const char buttons[] PROGMEM = "789456123A0P";

/************************************************************************/
/*                                                                      */
/*  Class Declaration                                                   */
/*                                                                      */
/************************************************************************/

/*  AdaSetTimePage::AdaSetTime
 *
 *      Build me
 */

AdaSetTimePage::AdaSetTimePage() : AdaUIPage(&ATime)
{
    offset = 0;
    changed = false;
}

AdaSetTimePage::AdaSetTimePage(uint8_t off, const AdaPage *p) : AdaUIPage(p)
{
    offset = off;
    changed = false;
}

void AdaSetTimePage::viewWillAppear()
{
    curPos = 0;
    changed = false;
    
    isPM = (hour >= 12); 
    hour %= 12;
    if (hour == 0) hour = 12;
    if (hour < 10) digit[0] = ' ';
    else digit[0] = '1';
    digit[1] = '0' + (hour % 10);
    digit[2] = '0' + (minute / 10);
    digit[3] = '0' + (minute % 10);    
}

void AdaSetTimePage::viewWillDisappear()
{
    if (changed) {
        if (hour == 12) hour = 0;
        if (isPM) hour += 12;
    }
}

/*  AdaSetTimePage::drawTime
 *
 *      Draw the selected time
 */

void AdaSetTimePage::drawTime()
{
    char buffer[12];
    
    buffer[0] = digit[0];
    buffer[1] = digit[1];
    buffer[2] = ':';
    buffer[3] = digit[2];
    buffer[4] = digit[3];
    buffer[5] = ' ';
    buffer[6] = isPM ? 'P' : 'A';
    buffer[7] = 'M';
    buffer[8] = 0;
    
    GC.setFont(&Narrow25D);
    GC.setTextColor(ADAUI_RED,ADAUI_BLACK);
    GC.drawButton(RECT(104+offset,50,113,37),buffer,28,0,KCenterAlign);
}

void AdaSetTimePage::drawKeyboard()
{
    char c[2];
    AdaUICorner corner;
    
    c[1] = 0;
    
    /*
     *  Draw keyboard
     */
    
    GC.setFont(&Narrow25D);
    for (uint8_t i = 0; i < 12; ++i) {
        uint8_t x = i % 3;
        uint8_t y = i / 3;
        char ch = (char)pgm_read_byte(buttons+i);
        c[0] = ch;
        switch (i) {
            case 0:
                corner = KCornerUL;
                break;
            case 2:
                corner = KCornerUR;
                break;
            case 9:
                corner = KCornerLL;
                break;
            case 11:
                corner = KCornerLR;
                break;
            default:
                corner = 0;
        }
        
        /*
         *  Determine positional-dependent digit
         */
        
        bool valid = false;
        if ((ch == 'A') || (ch == 'P')) valid = true;
        else {
            if (curPos == 0) {
                if (ch < '2') valid = true;
            } else if (curPos == 1) {
                if (digit[0] == ' ') valid = true;
                else if (ch < '3') valid = true;
            } else if (curPos == 2) {
                if (ch < '6') valid = true;
            } else {
                valid = true;
            }
        }
        
        if (valid) {
            GC.setTextColor(ADAUI_BLACK,ADAUI_BLUE);
        } else {
            GC.setTextColor(ADAUI_RED,ADAUI_DARKGRAY);
        }
        GC.drawButton(RECT(104+x*38+offset,89+y*38,37,37),c,28,corner,KCenterAlign);
    }
}

/*  AdaSetTimePage::drawContents
 *
 *      Draw the contents
 */

void AdaSetTimePage::drawContents()
{
    drawTime();
    drawKeyboard();
}

/*  AdaSetTimePage::handleTap
 *
 *      Handle keyboard tap event
 */

void AdaSetTimePage::handleTap(TS_Point pt)
{
    if (pt.x < 104+offset) return;
    if (pt.y < 89) return;
    
    uint8_t x = (pt.x - 104 - offset)/38;
    uint8_t y = (pt.y - 89)/38;


    if (x >= 3) return;
    if (y >= 4) return;
    
    uint8_t index = x + y * 3;
    char c = (char)pgm_read_byte(buttons+index);
    
    if (c == 'A') {
        isPM = false;
    } else if (c == 'P') {
        isPM = true;
    } else {
        /*
         *  This is stupid. But every digit has its own story.
         */
        
        if (curPos == 0) {
            if (c >= '2') return;
            
            hour = 0;
            minute = 0;

            if (c == '0') digit[0] = ' ';
            else {
                digit[0] = '1';
                hour = 10;
            }
            digit[1] = '_';
            digit[2] = ' ';
            digit[3] = ' ';
            curPos = 1;
        } else if (curPos == 1) {
            if ((digit[0] != ' ') && (c >= '3')) return;
            digit[1] = c;
            digit[2] = '_';
            hour += (c - '0');
            curPos = 2;
        } else if (curPos == 2) {
            if (c >= '6') return;
            digit[2] = c;
            digit[3] = '_';
            minute = 10 * (c - '0');
            curPos = 3;
        } else {
            digit[3] = c;
            minute += (c - '0');
            curPos = 0;
        }
    }
    changed = true;
    drawTime();
    drawKeyboard();
}


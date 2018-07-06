/*  AdaUtilsPage.cpp
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

#include "AdaUtils.h"
#include "AdaTime.h"
#include "AdaProgmem.h"
#include "AdaStrings.h"

/************************************************************************/
/*                                                                      */
/*  Utilities                                                           */
/*                                                                      */
/************************************************************************/

/*  PtInRect
 *
 *      Is point in rect?
 */

bool PtInRect(TS_Point pt, AdaUIRect r)
{
    if (pt.x < r.x) return false;
    if (pt.y < r.y) return false;
    if (pt.x > r.x + r.w) return false;
    if (pt.y > r.y + r.h) return false;
    return true;
}

void FormatNumber(char *dest, uint16_t temp)
{
    uint8_t pos = 0;
    uint8_t alt;
    
    if (temp == 0) {
        *dest++ = '0';
        *dest++ = 0;
    } else {
        while (temp > 0) {
            dest[pos++] = '0' + (temp % 10);
            temp /= 10;
        }
        alt = 0;
        dest[pos] = 0;
        --pos;
        while (alt < pos) {
            char c = dest[alt];
            dest[alt] = dest[pos];
            dest[pos] = c;
            ++alt;
            --pos;
        }
    }
}

void FormatTime(char *ptr, uint8_t hour, uint8_t minute)
{
    bool pm;

    pm = hour >= 12;
    hour %= 12;
    if (hour == 0) hour = 12;
    
    // Format hour/minute
    if (hour >= 10) {
        *ptr++ = '1';
        hour %= 10;
    }
    *ptr++ = '0' + hour;
    *ptr++ = ':';
    *ptr++ = '0' + (minute / 10);
    *ptr++ = '0' + (minute % 10);
    *ptr++ = ' ';
    if (pm) {
        *ptr++ = 'P';
    } else {
        *ptr++ = 'A';
    }
    *ptr++ = 'M';
    *ptr = 0;
}

/*
 *  Utility; append char from progmem
 */

static char *AppendChar(char *ptr, const __FlashStringHelper *ref)
{
    char ch;
    const char *r = (const char *)ref;
    
    while (0 != (ch = (char)pgm_read_byte(r++))) *ptr++ = ch;
    *ptr++ = ' ';
    return ptr;
}

static char *FormatDay(char *ptr, uint8_t dow)
{
    return AppendChar(ptr,GDOWString(dow));
}

void FormatDayTime(char *ptr, uint32_t time)
{
    AdaTimeRecord tr = AdaSecToTime(time);
    ptr = FormatDay(ptr,tr.dow);
    FormatTime(ptr,tr.hour,tr.min);
}

void FormatTime(char *buffer, uint32_t time)
{
    AdaTimeRecord tr = AdaSecToTime(time);
    FormatTime(buffer,tr.hour,tr.min);
}

void FormatDate(char *ptr, uint32_t time)
{
    AdaTimeRecord tr = AdaSecToTime(time);
    
    ptr = FormatDay(ptr,tr.dow);
    
    ptr = AppendChar(ptr,GMonthString(tr.month - 1));
    
    if (tr.day >= 10) {
        *ptr++ = '0' + tr.day/10;
    }
    *ptr++ = '0' + (tr.day % 10);
    *ptr++ = ',';
    *ptr++ = ' ';
    
    char buffer[8];
    FormatNumber(buffer,tr.year);
    for (char *c = buffer; *c; ++c) *ptr++ = *c;
    *ptr = 0;
}

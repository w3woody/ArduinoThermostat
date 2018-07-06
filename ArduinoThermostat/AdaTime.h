/*  AdaTime.h
 *
 *      Provide a way to track the current time on the Arduino.
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

#ifndef _AdaTime_h
#define _AdaTime_h

#include <stdint.h>

/************************************************************************/
/*                                                                      */
/*  Time Record                                                         */
/*                                                                      */
/************************************************************************/

struct AdaTimeRecord
{
    uint8_t sec;
    uint8_t min;
    uint8_t hour;
    uint8_t dow;            // 0 == sunday
    uint8_t day;
    uint8_t month;
    uint16_t year;
};

/************************************************************************/
/*                                                                      */
/*  Time/Clock                                                          */
/*                                                                      */
/************************************************************************/

extern void AdaTimeInitialize();

extern uint32_t AdaGetTime();
extern void AdaSetTime(uint32_t t);

/*
 *  Day/date conversion
 */

extern uint32_t AdaTimeOfDay(uint32_t time);
extern uint8_t AdaDayOfWeek(uint32_t time);     // 0 == SUN
extern AdaTimeRecord AdaSecToTime(uint32_t time);
extern uint32_t AdaTimeToSec(const AdaTimeRecord &t);

/*
 *  Date support utils
 */

extern uint32_t AdaGregorianDayCount(uint8_t day, uint8_t month, uint16_t year);
extern uint8_t AdaCalcDOW(uint8_t day, uint8_t month, uint16_t year);

#endif

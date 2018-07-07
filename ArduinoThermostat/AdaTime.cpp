/*  AdaTime.cpp
 *
 *      Sets up TIMER2 to be a real-time interrupt to count the number of
 *  elapsed seconds.
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

#include "AdaTime.h"

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <Arduino.h>
#include <pins_arduino.h>

/************************************************************************/
/*                                                                      */
/*  Globals                                                             */
/*                                                                      */
/************************************************************************/

static uint8_t GIncr;           // Increment management
static uint32_t GTime;          // # of seconds since 1/1/2017
static uint32_t GElapsed;       // # of seconds since power on

/************************************************************************/
/*                                                                      */
/*  Interrupt                                                           */
/*                                                                      */
/************************************************************************/

ISR(TIMER2_COMPA_vect)
{
    if (++GIncr >= 125) {
        GIncr = 0;
        ++GTime;
        ++GElapsed;
    }
}

/************************************************************************/
/*                                                                      */
/*  AdaTime                                                             */
/*                                                                      */
/************************************************************************/

/*  AdaTimeInitialize
 *
 *      Initalize TIMER2 for a 125hz output. (Assumption: 16mhz processor)
 */

void AdaTimeInitialize()
{
    // Template from https://forum.arduino.cc/index.php?topic=45276.0

    // Turn off timer.    
    TCCR2B &= ~7;       // TCCR2B bits 0,1,2 set to 0.

    // Set to CTC generation mode (WGM bits = 010)
    TCCR2A = (TCCR2A & ~0x3) | 0x2;
    TCCR2B &= ~0x08;
    
    // Configure action to perform by disconnecting from pins
    // COM2A = 00, COM2B = 00
    TCCR2A &= ~0xF0;
    
    // Reset counter to start at 0
    TCNT2 = 0;

    // Reset interrupt to occur after time has gone around once
    // Clear the interrupt flag. Set OCF2A to 0
    TIFR2 &= ~0x2;

    // Enable the interrupt when A match enabled
    // OCIE2A set to 1
    TIMSK2 = 0x2;
    
    //  Set the prescaler for 125hz. This is the best we can do
    //  to get us to a target 1hz counter
    //
    //  We set prescaler to 1024, and OCR2A to 125; this gives us
    //  the output of
    //
    //      processor / (prescaler * (output_compare + 1))
    //
    //      16000000 / (1024 * (124 + 1)) = 125 hz
    
    OCR2A = 124;
    TCCR2B |= 0x7;          // This also starts the clock running.
}

/*  AdaTime::setTime
 *
 *      Set the current time
 */

void AdaSetTime(uint32_t time)
{
    uint8_t oldSREG = SREG;
    cli();
    
    GTime = time;
    
    SREG = oldSREG;
}

uint32_t AdaGetTime()
{
    unsigned long ret;
    
    uint8_t oldSREG = SREG;
    cli();
    
    ret = GTime;
    
    SREG = oldSREG;
    
    return ret;
}

uint32_t AdaGetElapsedTime()
{
    unsigned long ret;
    
    uint8_t oldSREG = SREG;
    cli();
    
    ret = GElapsed;
    
    SREG = oldSREG;
    
    return ret;
}


/************************************************************************/
/*                                                                      */
/*  Gregorian Calendar                                                  */
/*                                                                      */
/************************************************************************/

#define EPOCH	736330			// 1/1/2017, which is a Sunday, makes DOW esay


static int8_t IsLeapYear(uint16_t year)
{
	if (year % 4) return 0;			/* like 2017 */
	if (year % 100) return 1;		/* like 1996 */
	if (year % 400) return 0;		/* like 1900 */
	return 1;						/* like 2000 */
}

uint32_t AdaGregorianDayCount(uint8_t day, uint8_t month, uint16_t year)
{
	uint16_t y1 = year - 1;
	uint32_t tmp;

	tmp = 365 * ((uint32_t)y1);
	tmp += y1/4;
	tmp -= y1/100;
	tmp += y1/400;
	tmp += (367 * (uint16_t)month - 362)/12;

	if (month > 2) {
		tmp -= IsLeapYear(year) ? 1 : 2;
	}

	return tmp + day - EPOCH;
}

static uint16_t GregorianYear(uint32_t dcount)
{
	uint32_t d;

	d = dcount + EPOCH - 1;
	uint8_t  n400 = d / 146097;

	d %= 146097;
	uint8_t  n100 = d / 36524;

	d %= 36524;
	uint8_t  n4   = d / 1461;

	d %= 1461;
	uint8_t  n1   = d / 365;

	int16_t year = 400 * (uint16_t)n400 + 100 * (uint16_t)n100 + 4 * n4 + n1;
	if ((n100 == 4) || (n1 == 4)) return year;
	return year + 1;
}

/*
 *  Set just the day/month/year and dow fields
 */

void GregorianDate(uint32_t dcount, AdaTimeRecord &tr)
{
	tr.year = GregorianYear(dcount);
	uint16_t priorDays = dcount - AdaGregorianDayCount(1,1,tr.year);
	uint8_t correction;
	uint32_t march = AdaGregorianDayCount(1,3,tr.year);	// march 1

	if (dcount < march) {
		correction = 0;
	} else if (IsLeapYear(tr.year)) {
		correction = 1;
	} else {
		correction = 2;
	}

	tr.month = (12 * (priorDays + correction) + 373)/367;
	tr.day = dcount - AdaGregorianDayCount(1,tr.month,tr.year) + 1;
	
	// Get the day of week. Epoch (1/1/2017) was Sunday...
	tr.dow = dcount % 7;
}


/************************************************************************/
/*                                                                      */
/*  Date/Time Conversion                                                */
/*                                                                      */
/************************************************************************/

uint32_t AdaTimeOfDay(uint32_t time)
{
    return time % 86400;            // 24 * 60 * 60 = 86400
}

uint8_t AdaDayOfWeek(uint32_t time)
{
    return (time / 86400) % 7;      // 0 = Sunday
}

AdaTimeRecord AdaSecToTime(uint32_t time)
{
    AdaTimeRecord tr;
    
    tr.sec = time % 60;
    time /= 60;
    tr.min = time % 60;
    time /= 60;
    tr.hour = time % 24;
    time /= 24;
    
    GregorianDate(time,tr);     // Find dow, day, month, year
    
    return tr;
}

uint32_t AdaTimeToSec(const AdaTimeRecord &tr)
{
    uint32_t dcount = AdaGregorianDayCount(tr.day,tr.month,tr.year);
    dcount = (dcount * 24) + tr.hour;
    dcount = (dcount * 60) + tr.min;
    dcount = (dcount * 60) + tr.sec;
    return dcount;
}

uint8_t AdaCalcDOW(uint8_t day, uint8_t month, uint16_t year)
{
    return AdaGregorianDayCount(day,month,year) % 7;
}
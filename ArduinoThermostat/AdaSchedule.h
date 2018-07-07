/*  AdaSchedule.h
 *
 *      The entry points for the schedule handler. Ideally this should
 *  store its values in NVRAM to survive power cycles, but this is just
 *  test code.
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

#ifndef _AdaSchedule_h
#define _AdaSchedule_h

#include <stdint.h>
#include "AdaUI.h"

/************************************************************************/
/*                                                                      */
/*  Structures                                                          */
/*                                                                      */
/************************************************************************/

/*  AdaScheduleItem
 *
 *      Marks a single item on our schedule
 */

struct AdaScheduleItem
{
    uint8_t hour;           /* 0-23 = hour of day, 0xFF = skip */
    uint8_t minute;         /* 0-59 = minute */
    uint8_t heat;           /* temperature to heat to */
    uint8_t cool;           /* temperature to cool to */
};

struct AdaScheduleDay
{
    struct AdaScheduleItem setting[4];  // 4 settings per day
};

/************************************************************************/
/*                                                                      */
/*  Class                                                               */
/*                                                                      */
/************************************************************************/

/*  AdaSchedule 
 *
 *      Periodically determines if we need to update our schedule. This
 *  only checks about once a second
 */

class AdaSchedule
{
    public:
                        AdaSchedule();

        void            periodicUpdate();
        
        /*
         *  Schedule. We have 5:
         *
         *      0: Spring
         *      1: Summer
         *      2: Fall
         *      3: Winter
         *      4: Energy Saver
         */
        
        void            setCurSchedule(uint8_t value);
        uint8_t         getCurSchedule();
                            
        AdaScheduleDay  getSchedule(uint8_t schedule, uint8_t dow);
        void            setSchedule(uint8_t schedule, uint8_t dow, const AdaScheduleDay &item);
    
    private:
        uint8_t         lastTrippedDOW;     /* The last setting we tripped */
        uint8_t         lastTrippedIndex;
        uint32_t        lastCheck;          /* The last time we checked */
};

/************************************************************************/
/*                                                                      */
/*  Globals                                                             */
/*                                                                      */
/************************************************************************/

/*
 *  Our global singleton which actually controls our hardware.
 */

extern AdaSchedule GSchedule;

#endif

/*  AdaSchedule.cpp
 *
 *      This is a dummy routine which varies the temperature depending
 *  on the time. Ideally this would be hooked up to a temperature sensor
 *  and an HVAC control system.
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

#include <stdint.h>
#include <EEPROM.h>
#include "AdaSchedule.h"
#include "AdaThermostat.h"
#include "AdaTime.h"

/*
 *  Note that we store our schedule in EEPROM. This means we need for
 *  our five separate schedules around 560 bytes of EEPROM. The Arduino
 *  Metro holds 1K so we're good, but other Arduinos only have 512 bytes,
 *  which is no bueno. You would have to make adjustments in the code
 *  here (and elsewhere) to make this fit.
 */

/************************************************************************/
/*                                                                      */
/*  Magic Value                                                         */
/*                                                                      */
/************************************************************************/

#define EEPROM_MAGIC        0x3C        // If we don't see this, reset.
#define EEPROM_MAGICADDR    (35 * sizeof(AdaScheduleDay))
#define EEPROM_SCHEDULE     (EEPROM_MAGICADDR + 1)

/************************************************************************/
/*                                                                      */
/*  Global Variables                                                    */
/*                                                                      */
/************************************************************************/

AdaSchedule GSchedule;

/************************************************************************/
/*                                                                      */
/*  Class Declaration                                                   */
/*                                                                      */
/************************************************************************/

/*  AdaSchedule::AdaSchedule
 *
 *      Set up our thermostat. Note this gets called on power-up, so we
 *  do our initialization, which sets up the EEPROM and sets the current
 *  temperature according to our schedule.
 */

AdaSchedule::AdaSchedule()
{
    if (EEPROM_MAGIC != EEPROM.read(EEPROM_MAGICADDR)) {
        AdaScheduleDay day;
        day.setting[0] = (AdaScheduleItem){  4,  0, 68, 78 };
        day.setting[1] = (AdaScheduleItem){  8, 30, 65, 82 };
        day.setting[2] = (AdaScheduleItem){ 16, 30, 68, 78 };
        day.setting[3] = (AdaScheduleItem){ 22,  0, 65, 76 };

        for (uint8_t i = 0; i < 5; ++i) {
            for (uint8_t dow = 0; dow < 7; ++dow) {
                setSchedule(i,dow,day);
            }
        }

        // And set to the energy saver
        setCurSchedule(4);  /* Energy saver default */

        // Do this last in case we crash above.
        EEPROM.update(EEPROM_MAGICADDR,EEPROM_MAGIC);
    } else {
        /*
         *  This will effectively set the current temperature according
         *  to our schedule
         */
        
        setCurSchedule(getCurSchedule());
    }
    
    /*
     *  Reset our check time so we spin our check
     */

    lastCheck = 0;
}

static uint16_t EEPROMAddr(uint8_t schedule, uint8_t dow)
{
    return (schedule * 7 + dow) * sizeof(AdaScheduleDay);
}

AdaScheduleDay AdaSchedule::getSchedule(uint8_t schedule, uint8_t dow)
{
    AdaScheduleDay ret;
    EEPROM.get(EEPROMAddr(schedule,dow),ret);
    return ret;
}

void AdaSchedule::setSchedule(uint8_t schedule, uint8_t dow, const AdaScheduleDay &item)
{
    EEPROM.put(EEPROMAddr(schedule,dow),item);
}

uint8_t AdaSchedule::getCurSchedule()
{
    return EEPROM.read(EEPROM_SCHEDULE);
}

void AdaSchedule::setCurSchedule(uint8_t value)
{
    /*
     *  Update the schedule, and show which schedule we have that is
     *  now controlling--even if it's completely blank.
     */
    
    EEPROM.update(EEPROM_SCHEDULE,value);
    GThermostat.lastSet = value;
    
    /*
     *  Now scan the schedule to find our temperatures. We scan the entire
     *  week to figure out the entry that would have set this temperature. If
     *  none, we don't do anything.
     */
    
    AdaTimeRecord tr = AdaSecToTime(AdaGetTime());
    uint8_t foundDOW = 0xFF;                // Found matching item
    uint8_t foundItem = 0xFF;
    uint8_t lastDOW = 0xFF;                 // Last found item
    uint8_t lastItem = 0xFF;
    bool found = false;
    
    for (uint8_t dow = 0; dow < 7; ++dow) {
        AdaScheduleDay d = getSchedule(value,tr.dow);
        for (uint8_t i = 0; i < 4; ++i, ++i) {
            uint8_t h = d.setting[i].hour;
            uint8_t m = d.setting[i].minute;
            if (h == 0xFF) continue;        // Skip if cleared.
            
            /*
             *  Determine if the date/time is past our current date/time.
             *  This means the last found entry is the entry that is the
             *  one we want.
             *
             *  We found our entry when the entry date/time is greater than
             *  our current date/time--and that flags the entry before this
             *  one is the one we want.
             */
            
            if (!found) {
                if (tr.dow < dow) found = true;
                else if (tr.dow == dow) {
                    if (tr.hour < h) found = true;
                    else if (tr.hour == h) {
                        if (tr.min < m) found = true;
                    }
                }
                
                if (found) {
                    /*
                     *  If the time is greater than the current time, then
                     *  the last entry is the entry we want
                     */
                    
                    foundDOW = lastDOW;
                    foundItem = lastItem;
                }
            }
            
            /*
             *  Now note the last dow and item we actually found a setting.
             */
            
            lastDOW = dow;
            lastItem = i;
        }
    }
    
    /*
     *  Determine if we ever had an entry. If not, bail
     */
    
    if (lastDOW == 0xFF) {
        lastTrippedDOW = 0xFF;
        lastTrippedIndex = 0xFF;
        return;
    }
    
    /*
     *  Now get the day and item of our schedule
     */
    
    if (foundDOW == 0xFF) {
        /*
         *  This is the case where the controlling item is the last item in
         *  our schedule.
         */
        
        foundDOW = lastDOW;
        foundItem = lastItem;
    }
    
    /*
     *  Finally! Set our thermostat.
     */

    AdaScheduleDay d = getSchedule(value,foundDOW);

    GThermostat.heatSetting = d.setting[foundItem].heat;
    GThermostat.coolSetting = d.setting[foundItem].cool;
    lastTrippedIndex = foundItem;
    lastTrippedDOW = foundDOW;
}

/*  AdaSchedule::periodicUpdate
 *
 *      We get called periodically; this should determine if we're
 *  running our HVAC, the delay time, etc. Note that our logic applies
 *  a delay to changing any state--our heater, cooler or fan. (See the
 *  notes in our header for more information.)
 */

void AdaSchedule::periodicUpdate()
{
    uint32_t time = AdaGetTime();
    if (time <= lastCheck) return;
    lastCheck = time;
    
    AdaTimeRecord tr = AdaSecToTime(time);
    AdaScheduleDay d = getSchedule(getCurSchedule(),tr.dow);
    
    /*
     *  This finds the item in the day. The way we work is to find the
     *  row that matches the hour/minute (as we check once a second so we
     *  have like 60 chances to match), and we set a note as to the last
     *  item we matched.
     *
     *  Note that the last trip item ranges from 0 to 27 and is dow * 4 + index
     *
     *  Note too all we do is change the temperature settings. We don't change
     *  any of the other thermostat settings.
     */
    
    for (uint8_t i = 0; i < 4; ++i) {
        if ((tr.dow == lastTrippedDOW) && (i == lastTrippedIndex)) continue;
        
        if ((d.setting[i].hour == tr.hour) && (d.setting[i].minute == tr.min)) {
            lastTrippedIndex = i;
            lastTrippedDOW = tr.dow;
            
            GThermostat.heatSetting = d.setting[i].heat;
            GThermostat.coolSetting = d.setting[i].cool;
            GThermostat.lastSet = getCurSchedule();
            break;
        }
    }
}

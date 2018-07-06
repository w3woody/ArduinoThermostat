/*  AdaStrings.cpp
 *
 *      Constant strings in our system. We move all our strings here because,
 *  given how large our application is, we want to localize the strings to
 *  one place so we don't create duplicate strings scattered throughout memory.
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

#include "AdaStrings.h"
#include "AdaProgmem.h"

/************************************************************************/
/*                                                                      */
/*  Strings                                                             */
/*                                                                      */
/************************************************************************/

/*
 *  Months
 */

const char string_jan[] PROGMEM = "JAN";
const char string_feb[] PROGMEM = "FEB";
const char string_mar[] PROGMEM = "MAR";
const char string_apr[] PROGMEM = "APR";
const char string_may[] PROGMEM = "MAY";
const char string_jun[] PROGMEM = "JUN";
const char string_jul[] PROGMEM = "JUL";
const char string_aug[] PROGMEM = "AUG";
const char string_sep[] PROGMEM = "SEP";
const char string_oct[] PROGMEM = "OCT";
const char string_nov[] PROGMEM = "NOV";
const char string_dec[] PROGMEM = "DEC";

const char * const GStringMonths[] PROGMEM = {
    string_jan, string_feb, string_mar,
    string_apr, string_may, string_jun,
    string_jul, string_aug, string_sep,
    string_oct, string_nov, string_dec
};

/*
 *  Helpers
 */

const __FlashStringHelper *GMonthString(uint8_t ix)
{
    return (const __FlashStringHelper *)pgm_read_pointer(GStringMonths + ix);
}

/*
 *  Day of week
 */

const char string_sun[] PROGMEM = "SUN";
const char string_mon[] PROGMEM = "MON";
const char string_tue[] PROGMEM = "TUE";
const char string_wed[] PROGMEM = "WED";
const char string_thu[] PROGMEM = "THU";
const char string_fri[] PROGMEM = "FRI";
const char string_sat[] PROGMEM = "SAT";

const char * const GStringDOW[] PROGMEM = {
    string_sun, string_mon, string_tue,
    string_wed, string_thu, string_fri,
    string_sat    
};


/*
 *  Helpers
 */

const __FlashStringHelper *GDOWString(uint8_t ix)
{
    return (const __FlashStringHelper *)pgm_read_pointer(GStringDOW + ix);
}

/*
 *  Schedule settings
 */

const char string_spring[] PROGMEM = "SPRING";
const char string_summer[] PROGMEM = "SUMMER";
const char string_fall[]   PROGMEM = "FALL";
const char string_winter[] PROGMEM = "WINTER";
const char string_saver[]  PROGMEM = "SAVER";

const char* const GScheduleNames[] PROGMEM = {
   string_spring, string_summer, string_fall, string_winter, string_saver
};

const __FlashStringHelper *GScheduleString(uint8_t ix)
{
    return (const __FlashStringHelper *)pgm_read_pointer(GScheduleNames + ix);
}

/************************************************************************/
/*                                                                      */
/*  Miscellaneous Strings                                               */
/*                                                                      */
/************************************************************************/

const char string_fan[] PROGMEM = "FAN";
const char string_schedule[] PROGMEM = "SCHEDULE";
const char string_settings[] PROGMEM = "SETTINGS";
const char string_back[] PROGMEM = "\177DONE";
const char string_edit[] PROGMEM = "EDIT";

const char string_set_date[] PROGMEM = "SET DATE";
const char string_set_time[] PROGMEM = "SET TIME";
const char string_date[] PROGMEM = "DATE";
const char string_time[] PROGMEM = "TIME";

const char string_plus[] PROGMEM = "+";
const char string_minus[] PROGMEM = "-";

const char string_auto[] PROGMEM = "AUTO";
const char string_on[] PROGMEM = "ON";
const char string_off[] PROGMEM = "OFF";

const char string_clear[] PROGMEM = "CLEAR";
const char string_copy[] PROGMEM = "COPY";
const char string_paste[] PROGMEM = "PASTE";

/*  AdaStrings.h
 *
 *      These are all the strings declared in our system. This allows us
 *  to consolidate strings in one place, so we don't have duplicates
 *  scattered throughout our memory.
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

#ifndef _AdaStrings_h
#define _AdaStrings_h

#include <WString.h>

/************************************************************************/
/*                                                                      */
/*  Constants                                                           */
/*                                                                      */
/************************************************************************/

/*
 *  Months
 */

extern const char string_jan[];
extern const char string_feb[];
extern const char string_mar[];
extern const char string_apr[];
extern const char string_may[];
extern const char string_jun[];
extern const char string_jul[];
extern const char string_aug[];
extern const char string_sep[];
extern const char string_oct[];
extern const char string_nov[];
extern const char string_dec[];
extern const char * const GStringMonths[];

extern const __FlashStringHelper *GMonthString(uint8_t ix);

/*
 *  Day of week
 */

extern const char string_sun[];
extern const char string_mon[];
extern const char string_tue[];
extern const char string_wed[];
extern const char string_thu[];
extern const char string_fri[];
extern const char string_sat[];
extern const char * const GStringDOW[];

extern const __FlashStringHelper *GDOWString(uint8_t ix);

/*
 *  Schedule strings
 */

extern const char string_spring[];
extern const char string_summer[];
extern const char string_fall[];
extern const char string_winter[];
extern const char string_saver[];

extern const char* const GScheduleNames[];

extern const __FlashStringHelper *GScheduleString(uint8_t ix);

#endif

/*
 *  Miscellaneous strings
 */

extern const char string_fan[];
extern const char string_schedule[];
extern const char string_settings[];
extern const char string_schedule[];
extern const char string_set_date[];
extern const char string_set_time[];
extern const char string_date[];
extern const char string_time[];
extern const char string_back[];
extern const char string_edit[];
extern const char string_plus[];
extern const char string_minus[];
extern const char string_auto[];
extern const char string_on[];
extern const char string_off[];
extern const char string_clear[];
extern const char string_copy[];
extern const char string_paste[];

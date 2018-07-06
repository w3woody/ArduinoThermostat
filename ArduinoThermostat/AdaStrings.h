/*  AdaStrings.h
 *
 *      These are all the strings declared in our system. This allows us
 *  to consolidate strings in one place, so we don't have duplicates
 *  scattered throughout our memory.
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

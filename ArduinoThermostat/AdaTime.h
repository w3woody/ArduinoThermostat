/*  AdaTime.h
 *
 *      Provide a way to track the current time on the Arduino.
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

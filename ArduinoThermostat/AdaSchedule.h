/*  AdaSchedule.h
 *
 *      The entry points for the schedule handler. Ideally this should
 *  store its values in NVRAM to survive power cycles, but this is just
 *  test code.
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
         *  Schedule. We have 3: Winter, Summer, Saving
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

/*  AdaSchedule.cpp
 *
 *      This is a dummy routine which varies the temperature depending
 *  on the time. Ideally this would be hooked up to a temperature sensor
 *  and an HVAC control system.
 */

#include <stdint.h>
#include "AdaSchedule.h"
#include "AdaThermostat.h"
#include "AdaTime.h"

/************************************************************************/
/*                                                                      */
/*  Global Variables                                                    */
/*                                                                      */
/************************************************************************/

AdaSchedule GSchedule;

const char GStringSpring[] PROGMEM = "SPRING";
const char GStringSummer[] PROGMEM = "SUMMER";
const char GStringFall[]   PROGMEM = "FALL";
const char GStringWinter[] PROGMEM = "WINTER";
const char GStringSaver[]  PROGMEM = "SAVER";

const char* const GScheduleName[] PROGMEM = {
   GStringSpring, GStringSummer, GStringFall, GStringWinter, GStringSaver
};


/************************************************************************/
/*                                                                      */
/*  Class Declaration                                                   */
/*                                                                      */
/************************************************************************/

/*  AdaSchedule::AdaSchedule
 *
 *      Set up our thermostat
 */

AdaSchedule::AdaSchedule()
{
    // ### TODO: Store in NVRAM. For now, slam values
    for (uint8_t i = 0; i < 4; ++i) {
        for (uint8_t dow = 0; dow < 7; ++dow) {
            AdaScheduleDay *d = schedules[i].dow + dow;
            d->setting[0] = (AdaScheduleItem){  4,  0, 68, 78 };
            d->setting[1] = (AdaScheduleItem){  8, 30, 65, 82 };
            d->setting[2] = (AdaScheduleItem){ 16, 30, 68, 78 };
            d->setting[3] = (AdaScheduleItem){ 22,  0, 65, 76 };
        }
    }
    for (uint8_t dow = 0; dow < 7; ++dow) {
        AdaScheduleDay *d = schedules[4].dow + dow;
        d->setting[0] = (AdaScheduleItem){  4,  0, 62, 85 };
        d->setting[1] = (AdaScheduleItem){  8, 30, 62, 85 };
        d->setting[2] = (AdaScheduleItem){ 16, 30, 62, 85 };
        d->setting[3] = (AdaScheduleItem){ 22,  0, 62, 85 };
    }
    
    curSchedule = 1;        /* Summer default */
    lastTripped = 0xFF;
    lastCheck = 0;
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
    AdaScheduleRecord *r = schedules + curSchedule;
    AdaScheduleDay *d = r->dow + tr.dow;
    
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
        uint8_t tripItem = i + tr.dow * 4;
        if (tripItem == lastTripped) continue;
        
        if ((d->setting[i].hour == tr.hour) && (d->setting[i].minute == tr.min)) {
            lastTripped = tripItem;
            GThermostat.heatSetting = d->setting[i].heat;
            GThermostat.coolSetting = d->setting[i].cool;
            GThermostat.lastSet = curSchedule;
        }
    }
}

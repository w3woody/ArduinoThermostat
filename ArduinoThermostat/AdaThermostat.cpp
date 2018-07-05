/*  AdaThermostat.cpp
 *
 *      This is a dummy routine which varies the temperature depending
 *  on the time. Ideally this would be hooked up to a temperature sensor
 *  and an HVAC control system.
 */

#include <stdint.h>
#include "AdaThermostat.h"
#include "AdaTime.h"

/************************************************************************/
/*                                                                      */
/*  Global Variables                                                    */
/*                                                                      */
/************************************************************************/

AdaThermostat GThermostat;

/************************************************************************/
/*                                                                      */
/*  Class Declaration                                                   */
/*                                                                      */
/************************************************************************/

/*  AdaThermostat::AdaThermostat
 *
 *      Set up our thermostat
 */

AdaThermostat::AdaThermostat()
{
    heatSetting = 68;       // Temperature to heat to when cold
    coolSetting = 78;       // Temperature to cool to when hot
    curTemperature = 75;    // Current temperature
    fanSetting = ADAHVAC_FAN_AUTO;  // Auto fan
    
    unitState = 0;          // Unit starts off
    delay = 0;              // Time when we can change the HVAC state.
    lastSet = 1;            // Who set me last? (0xFF == manual)
}

/*  AdaThermostat::periodicUpdate
 *
 *      We get called periodically; this should determine if we're
 *  running our HVAC, the delay time, etc. Note that our logic applies
 *  a delay to changing any state--our heater, cooler or fan. (See the
 *  notes in our header for more information.)
 */

void AdaThermostat::periodicUpdate()
{
    /*
     *  Get the current temperature from the sensor. This emulates 
     *  the sensor. This swings the temperature from 65 to 80, moving
     *  one degree per 10 seconds.
     */
    
    uint32_t t = AdaGetTime();
    uint8_t tmp = (t / 10) % 30;
    if (tmp < 15) {
        curTemperature = 65 + tmp;
    } else {
        curTemperature = 95 - tmp;  // 80 - (tmp - 15)
    }
    
    /*
     *  Determine if we need to run our HVAC or not.
     */
    
    uint8_t newState = 0;
    
    if (fanSetting != ADAHVAC_OFF) {
        /*
         *  Our logic guarantees we don't stupidly try to turn the heater
         *  and the HVAC on at the same time.
         */
        
        if (heatSetting - TEMPWINDOW >= curTemperature) {
            newState |= ADAHVAC_FAN | ADAHVAC_HEAT;
        } else if (coolSetting + TEMPWINDOW <= curTemperature) {
            newState |= ADAHVAC_FAN | ADAHVAC_COOL;
        }
        
        if (fanSetting == ADAHVAC_FAN_ON) {
            newState |= ADAHVAC_FAN;
        }
    }
    
    /*
     *  Now determine if we can even change our state.
     */
    
    if (newState != unitState) {
        uint32_t t = AdaGetTime();
        if (t > delay) {
            delay = t + MINCYCLE;
            unitState = newState;
        }
    }
    
    /*
     *  TODO: This is where we would use our unitState flags to actually
     *  control our hardware
     */
    
}

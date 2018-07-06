/*  AdaThermostat.cpp
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
    
    heat = false;           // Heat/cool/fan starts off
    cool = false;
    fan = false;

    delay = 0;              // Time when we can change the HVAC state.
    fanrun = 0;             // Time when we turn off the fan after HVAC turns off
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
     *  Get elapsed time since startup. We need this for our thermostat
     *  timing
     */
    
    uint32_t t = AdaGetElapsedTime();

    /*
     *  Get the current temperature from the sensor. This emulates 
     *  the sensor. This swings the temperature from 65 to 80, moving
     *  one degree per 10 seconds.
     */
    
    /* ### TODO: set curTemperature to a sensor. This is a kludge to
     * emulate a temperature bouncing up and down for testing
     */
    
    /* BEGIN REPLACE ME */
    uint8_t tmp = (t / 10) % 30;
    if (tmp < 15) {
        curTemperature = 65 + tmp;
    } else {
        curTemperature = 95 - tmp;  // 80 - (tmp - 15)
    }
    /* END REPLACE ME */
    
    /*
     *  Determine if we need to run our HVAC or not.
     */
    
    bool newHeat = false;
    bool newCool = false;
    
    if (fanSetting != ADAHVAC_OFF) {
        /*
         *  Our logic guarantees we don't stupidly try to turn the heater
         *  and the HVAC on at the same time. We also wait until after
         *  the last state change has taken place.
         */
        
        if (t > delay) {
            /*
             *  Only test to change if our old time has elapsed
             */
            
            if (heatSetting - TEMPWINDOW >= curTemperature) {
                newHeat = true;
                fan = true;
            } else if (coolSetting + TEMPWINDOW <= curTemperature) {
                newCool = true;
                fan = true;
            }
        } else {
            newHeat = heat;
            newCool = cool;
            fan = fan;
        }
        
    }

    /*
     *  Determine when we run the fan. This happens if (a) we just turned
     *  off the heater or cooler and the 'fanrun' time has not elapsed,
     *  (b) if the user has manually turned on the fan, or (c) if either
     *  the heater or cooler are now running.
     *
     *  We can put more sophisticated logic here if we want, such as turning
     *  on the fan a few seconds before turning on the heater or air
     *  conditioner, or even verifying the system is running properly before
     *  turning on different stages.
     */
     
    fan = newHeat || newCool || (fanSetting == ADAHVAC_FAN_ON) || (t <= fanrun);
    
    /*
     *  Now determine if we've changed our state. If we have, then note the
     *  time (for our delay), and see if the heater or cooler were just
     *  switched off so we turn the fan on.
     *
     *  Our logic is such that we do allow the fan to be immediately turned on
     *  and off, unless required by the heater or air conditioner.
     */
    
    if ((newHeat != heat) || (newCool != cool)) {
        delay = t + MINCYCLE;
        
        /*
         *  Detect if our we just turned off the heater or air conditioner
         */
        
        if ((!newHeat & heat) || (!newCool & cool)) {
            fan = true;
            fanrun = t + FANRUNWINDOW;
        }
        
        heat = newHeat;
        cool = newCool;
    }
    
    /*
     *  TODO: This is where we would use our unitState flags to actually
     *  control our hardware. Use the flags 'heat', 'cool', and 'fan' to
     *  set or clear the control lines on our Arduino.
     */
    
    /* BEGIN REPLACE ME */
    
    /*
     *  Ideally you would need three pins: one to turn on the fan, one to
     *  turn on the heater, and one to turn on the air conditioner.
     *
     *  The logic in the code above is such that the heater and the
     *  air conditioner will never be run at the same time, and the
     *  fan will always run if either the heater or the air conditioner
     *  is being run.
     *
     *  In practice, however, you would want to duplicate this logic in
     *  hardware to prevent malfunctions from putting your HVAC system in
     *  an illegal state. One possible logic diagram would be
     *
     *                       -------------
     *      fan_input ------ |           |
     *      heat_input ----- |    OR     | -------- fan_output
     *      cool_input ----- |           |
     *                       -------------
     *
     *      heat_input ---------------------------- heat_output
     *
     *                     -------    ----------
     *      heat_input ----| NOT | -- |        |
     *                     -------    |  AND   | -- cool_output
     *                                |        |
     *      cool_input ---------------|        |
     *                                ----------
     *
     *
     *  This circuit would guarantee the fan is always on if the Arduino
     *  signals turning on the fan, and make sure only the heater or
     *  the air conditioner is running, but not both. (This circuit favors
     *  the heater rather than the air conditioner, but you could flip the
     *  logic if you wish. You could use two AND gates and two NOT gates
     *  to make sure neither the heater or the air conditioner run if both
     *  lines are set--and you can use a third AND gate to illuminate an 
     *  LED if both lines are on.
     */
     
    /* END REPLACE ME */
}

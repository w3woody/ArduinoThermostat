/*  AdaThermostat.h
 *
 *      The entry points for the thermostat. This is just emulated for now
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

#ifndef _AdaThermostat_h
#define _AdaThermostat_h

#include <stdint.h>

/************************************************************************/
/*                                                                      */
/*  Constants                                                           */
/*                                                                      */
/************************************************************************/

/*  MINCYCLE
 *
 *      Minimum cycle time in seconds. This is the minimum amount of time
 *  we will wait to turn on or off the heater or the cooler. This is to
 *  prevent unnecessarily short power cycles sent to the HVAC to keep from
 *  damaging the unit.
 *
 *      In the real world this would be set a lot longer. We set it to 1 min
 *  for testing purposes.
 *
 *      Note: if any of this shit blows up your HVAC, don't blame me.
 *  This is demo code.
 */

#define MINCYCLE        60              /* Minimum cycle time in seconds */

/*  TEMPWINDOW
 *
 *      This is the +/- temperature window at which point we turn the
 *  HVAC unit on or off. If set to 1, this means when the heater is set to
 *  68, we actually turn on the heater at 67, and turn it off at 68. For
 *  the air conditioner set at 78, we actually turn it on at 79, and turn
 *  it off at 78.
 */

#define TEMPWINDOW      1               /* 1 degree window */

/************************************************************************/
/*                                                                      */
/*  Global Variables                                                    */
/*                                                                      */
/************************************************************************/

#define ADAHVAC_OFF         0           /* HVAC/fan always off */
#define ADAHVAC_FAN_AUTO    1           /* Fan only runs when HVAC on */
#define ADAHVAC_FAN_ON      2           /* Fan is always running */

/*
 *  unit state bits
 */

#define ADAHVAC_FAN     0x01            /* Set to true if fan running */
#define ADAHVAC_COOL    0x02            /* Set to true if cooling */
#define ADAHVAC_HEAT    0x04            /* Set to true if heating */

/************************************************************************/
/*                                                                      */
/*  Class                                                               */
/*                                                                      */
/************************************************************************/

class AdaThermostat
{
    public:
                        AdaThermostat();

        void            periodicUpdate();
        
        uint8_t         heatSetting;
        uint8_t         coolSetting;
        uint8_t         fanSetting;
        uint8_t         curTemperature;
        uint8_t         unitState;
        uint8_t         lastSet;
    
    private:
        uint32_t        delay;
};

/************************************************************************/
/*                                                                      */
/*  Globals                                                             */
/*                                                                      */
/************************************************************************/

/*
 *  Our global singleton which actually controls our hardware.
 */

extern AdaThermostat GThermostat;

#endif

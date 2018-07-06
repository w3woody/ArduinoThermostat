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

/*  FANRUNWINDOW
 *
 *      This is the number of seconds past the time the heater or the
 *  air conditioner turned off before we wind down the fan. In practice the
 *  HVAC system's heating elements or cooling elements remain hot or cold
 *  after the system is turned off--and we want to continue blowing across
 *  them so they don't overheat, and so we get the residual heat or cooling
 *  into the room where it belongs.
 */

#define FANRUNWINDOW    60              /* Run the fan for 60 seconds */

/************************************************************************/
/*                                                                      */
/*  Constants                                                           */
/*                                                                      */
/************************************************************************/

/*
 *  Legal values of GThermostat.fanSetting. Notice that field also doubles
 *  as a setting to turn the entire unit off.
 */

#define ADAHVAC_OFF         0           /* HVAC/fan always off */
#define ADAHVAC_FAN_AUTO    1           /* Fan only runs when HVAC on */
#define ADAHVAC_FAN_ON      2           /* Fan is always running */

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
        
        uint8_t         heatSetting;        /* Target heat to temperature */
        uint8_t         coolSetting;        /* Target cool to temperature */
        uint8_t         fanSetting;         /* ADAVAC_OFF/AUTO/ON */
        uint8_t         curTemperature;     /* Current interior temperature */
        uint8_t         lastSet;            /* Last schedule used or 0xFF */
        
        bool            heat;               /* True if heater runs */
        bool            cool;               /* True if aircon runs */
        bool            fan;                /* True if fan runs */
    
    private:
        uint32_t        delay;
        uint32_t        fanrun;
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

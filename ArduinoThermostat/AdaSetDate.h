/*  AdaSetDate.h
 *
 *      Allows the users to set the temperature of our thermostat.
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

#ifndef _AdaSetDate_h
#define _AdaSetDate_h

#include "AdaUIPage.h"
#include "AdaTime.h"

/************************************************************************/
/*                                                                      */
/*  Home page                                                           */
/*                                                                      */
/************************************************************************/

/*  AdaSetDatePage
 *
 *      Draws our thermostat and controls the basic settings.
 */

class AdaSetDatePage: public AdaUIPage
{
    public:
                        AdaSetDatePage();

        virtual void    drawContents();     // Override for custom contents

        virtual void    handleEvent(uint8_t ix);
        virtual void    handleTap(TS_Point pt);
        virtual void    viewWillAppear();

    private:
        void            drawMonth();
        void            drawYear();
        void            drawCalendar();
        
        void            setDate();
        
        uint8_t         day;
        uint8_t         month;
        uint16_t        year;
        
        uint8_t         mdow;
        uint8_t         mdays;
        uint8_t         mweeks;
        uint8_t         mheight;
};


#endif

/*  AdaSetTime.h
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

#ifndef _AdaSetTime_h
#define _AdaSetTime_h

#include "AdaUIPage.h"

/************************************************************************/
/*                                                                      */
/*  Callback                                                            */
/*                                                                      */
/************************************************************************/

typedef 

/************************************************************************/
/*                                                                      */
/*  Home page                                                           */
/*                                                                      */
/************************************************************************/

/*  AdaSetTime
 *
 *      Draws our basic time setting widget. This is deliberately designed
 *  so it can be overridden.
 */

class AdaSetTimePage: public AdaUIPage
{
    public:
                        AdaSetTimePage();
                        
        virtual void    drawContents();     // Override for custom contents

        virtual void    handleTap(TS_Point pt); // Allow other tap handling
        virtual void    viewWillAppear();
        virtual void    viewWillDisappear();
                
        uint8_t         hour;               // pointer to result array [2 bytes]
        uint8_t         minute;             // pointer to result array [2 bytes]
        bool            changed;            // Indicate we changed.
        
    protected:
        // Alternate constructor
                        AdaSetTimePage(uint8_t offset, const AdaPage *p);

    private:
        uint8_t         offset;             // Offset of our display
        uint8_t         curPos;
        bool            isPM;
        char            digit[4];
        
        void            drawTime();
        void            drawKeyboard();
};

#endif

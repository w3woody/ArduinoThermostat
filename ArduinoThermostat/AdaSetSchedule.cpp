/*  AdaSetSchedule.cpp
 *
 *      Defines the behavior of our home page, which displays the thermostat.
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

#include "AdaUIScreen.h"
#include "AdaSetSchedule.h"
#include "Narrow25D.h"
#include "AdaTime.h"
#include "AdaThermostat.h"
#include "AdaUtils.h"
#include "AdaProgmem.h"
#include "AdaStrings.h"

/************************************************************************/
/*                                                                      */
/*  Layout Constants                                                    */
/*                                                                      */
/************************************************************************/

static const AdaUIRect ATimeRects[] PROGMEM = {
    {  10, 89,40,37 },       // Heat--
    {  51, 89,40,37 },       // Heat++
    { 100, 89,40,37 },       // Cool--
    { 141, 89,40,37 }        // Cool++
};

static const AdaPage ATime PROGMEM = {
    string_schedule, string_back, NULL, ATimeRects, 4
};

/************************************************************************/
/*                                                                      */
/*  Class Declaration                                                   */
/*                                                                      */
/************************************************************************/

/*  AdaSetSchedulePage::AdaSetSchedule
 *
 *      Build me
 */

AdaSetSchedulePage::AdaSetSchedulePage() : AdaSetTimePage(86,&ATime)
{
}

/*  AdaSetSchedulePage::drawContents
 *
 *      Draw the contents
 */

void AdaSetSchedulePage::drawContents()
{
    drawHeat();
    drawCool();
    
    GC.setTextColor(ADAUI_BLACK,ADAUI_BLUE);

    GC.drawButton(RECT( 10, 89,40,37),(const __FlashStringHelper *)string_minus,28,KCornerUL | KCornerLL,KCenterAlign);
    GC.drawButton(RECT( 51, 89,40,37),(const __FlashStringHelper *)string_plus,28,KCornerUR | KCornerLR,KCenterAlign);
    GC.drawButton(RECT(100, 89,40,37),(const __FlashStringHelper *)string_minus,28,KCornerUL | KCornerLL,KCenterAlign);
    GC.drawButton(RECT(141, 89,40,37),(const __FlashStringHelper *)string_plus,28,KCornerUR | KCornerLR,KCenterAlign);

    AdaSetTimePage::drawContents();
}

void AdaSetSchedulePage::drawHeat()
{
    char buffer[8];
    
    FormatNumber(buffer,heat);
    GC.setFont(&Narrow25D);
    GC.setTextColor(ADAUI_RED,ADAUI_BLACK);
    GC.drawButton(RECT(10,50,80,37),buffer,28,0,KCenterAlign);
}

void AdaSetSchedulePage::drawCool()
{
    char buffer[8];
    
    FormatNumber(buffer,cool);
    GC.setFont(&Narrow25D);
    GC.setTextColor(ADAUI_RED,ADAUI_BLACK);
    GC.drawButton(RECT(100,50,80,37),buffer,28,0,KCenterAlign);
}

void AdaSetSchedulePage::handleEvent(uint8_t ix)
{
    switch (ix) {
        case AEVENT_FIRSTSPOT:
            if (heat > 50) {
                --heat;
                changed = true;
                drawHeat();
            }
            break;
        case AEVENT_FIRSTSPOT+1:
            if (heat + 5 < cool) {
                ++heat;
                changed = true;
                drawHeat();
            }
            break;
        case AEVENT_FIRSTSPOT+2:
            if (heat + 5 < cool) {
                --cool;
                changed = true;
                drawCool();
            }
            break;
        case AEVENT_FIRSTSPOT+3:
            if (cool < 90) {
                ++cool;
                changed = true;
                drawCool();
            }
            break;
    }
}

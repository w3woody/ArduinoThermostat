/*  AdaTempPage.cpp
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
#include "AdaTempPage.h"
#include "Narrow25D.h"
#include "Narrow75D.h"
#include "AdaTime.h"
#include "AdaThermostat.h"
#include "AdaUtils.h"
#include "AdaStrings.h"

/************************************************************************/
/*                                                                      */
/*  Layout Constants                                                    */
/*                                                                      */
/************************************************************************/

static const AdaUIRect ATempRects[] PROGMEM = {
    { 117,  88,  40,  37 },       // Heat ++
    { 117, 126,  40,  37 },       // Heat --
    { 231,  88,  40,  37 },       // Cool ++
    { 231, 126,  40,  37 },       // Cool --
    {  64, 195,  63,  37 },       // Auto
    { 148, 195,  63,  37 },       // On
    { 229, 195,  63,  37 }        // Off
};

static const AdaPage ATemp PROGMEM = {
    string_settings, string_back, NULL, ATempRects, 7
};

/************************************************************************/
/*                                                                      */
/*  Draw Support                                                        */
/*                                                                      */
/************************************************************************/

static void DrawHeatCool(uint16_t xoff, uint8_t temp)
{
    char buffer[8];

    GC.setFont(&Narrow75D);
    GC.setTextColor(ADAUI_RED,ADAUI_BLACK);

    FormatNumber(buffer,temp);
    GC.drawButton(RECT(xoff,88,70,75),buffer,66);
}

static void DrawFan(uint8_t fan)
{
    GC.setTextColor(ADAUI_BLACK,(fan == ADAHVAC_OFF) ? ADAUI_GREEN : ADAUI_DARKGRAY);
    GC.drawButton(RECT(209,195,19,37));

    GC.setTextColor(ADAUI_BLACK,(fan == ADAHVAC_FAN_AUTO) ? ADAUI_GREEN : ADAUI_DARKGRAY);
    GC.drawButton(RECT( 44,195,19,37));

    GC.setTextColor(ADAUI_BLACK,(fan == ADAHVAC_FAN_ON) ? ADAUI_GREEN : ADAUI_DARKGRAY);
    GC.drawButton(RECT(128,195,19,37));
}

/************************************************************************/
/*                                                                      */
/*  Class Declaration                                                   */
/*                                                                      */
/************************************************************************/

/*  AdaTempPage::AdaTempPage
 *
 *      Build me
 */

AdaTempPage::AdaTempPage() : AdaUIPage(&ATemp)
{
}

/*  AdaTempPage::drawContents
 *
 *      Draw the contents
 */

void AdaTempPage::drawContents()
{
    char buffer[8];

    // Draw temperatures
    DrawHeatCool( 43,GThermostat.heatSetting);
    DrawHeatCool(157,GThermostat.coolSetting);
    
    // Draw buttons
    GC.setFont(&Narrow25D);
    GC.setTextColor(ADAUI_BLACK,ADAUI_BLUE);
    GC.drawButton(RECT(117,88,40,37), (const __FlashStringHelper *)string_plus,28,KCornerUL | KCornerUR,KCenterAlign);
    GC.drawButton(RECT(117,126,40,37),(const __FlashStringHelper *)string_minus,28,KCornerLL | KCornerLR,KCenterAlign);

    GC.drawButton(RECT(231,88,40,37), (const __FlashStringHelper *)string_plus,28,KCornerUL | KCornerUR,KCenterAlign);
    GC.drawButton(RECT(231,126,40,37),(const __FlashStringHelper *)string_minus,28,KCornerLL | KCornerLR,KCenterAlign);
    
    // Draw state buttons
    GC.drawButton(RECT( 32,195,11,37),KCornerUL | KCornerLL);
    GC.drawButton(RECT( 64,195,63,37),(const __FlashStringHelper *)string_auto,28);
    GC.drawButton(RECT(148,195,60,37),(const __FlashStringHelper *)string_on,28);
    GC.drawButton(RECT(229,195,60,37),(const __FlashStringHelper *)string_off,28,KCornerUR | KCornerLR);
    
    DrawFan(GThermostat.fanSetting);
}

/*  AdaTempPage::handleEvent
 *
 *      Handle tap events
 */

void AdaTempPage::handleEvent(uint8_t ix)
{
    switch (ix) {
        case AEVENT_FIRSTSPOT:
            if (GThermostat.heatSetting+5 < GThermostat.coolSetting) {
                GThermostat.heatSetting++;
                GThermostat.lastSet = 0xFF;
                DrawHeatCool( 43,GThermostat.heatSetting);
            }
            break;
        case AEVENT_FIRSTSPOT+1:
            if (GThermostat.heatSetting > 50) {
                GThermostat.heatSetting--;
                GThermostat.lastSet = 0xFF;
                DrawHeatCool( 43,GThermostat.heatSetting);
            }
            break;
        case AEVENT_FIRSTSPOT+2:
            if (GThermostat.coolSetting < 90) {
                GThermostat.coolSetting++;
                GThermostat.lastSet = 0xFF;
                DrawHeatCool(157,GThermostat.coolSetting);
            }
            break;
        case AEVENT_FIRSTSPOT+3:
            if (GThermostat.heatSetting+5 < GThermostat.coolSetting) {
                GThermostat.coolSetting--;
                GThermostat.lastSet = 0xFF;
                DrawHeatCool(157,GThermostat.coolSetting);
            }
            break;
        case AEVENT_FIRSTSPOT+4:
            GThermostat.fanSetting = ADAHVAC_FAN_AUTO;
            DrawFan(GThermostat.fanSetting);
            break;
        case AEVENT_FIRSTSPOT+5:
            GThermostat.fanSetting = ADAHVAC_FAN_ON;
            DrawFan(GThermostat.fanSetting);
            break;
        case AEVENT_FIRSTSPOT+6:
            GThermostat.fanSetting = ADAHVAC_OFF;
            DrawFan(GThermostat.fanSetting);
            break;
    }
}

/*  AdaSettingsPage.cpp
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
#include "AdaSettingsPage.h"
#include "Narrow25D.h"
#include "AdaTime.h"
#include "AdaThermostat.h"
#include "AdaUtils.h"

#include "AdaSetDate.h"
#include "AdaSetTime.h"
#include "AdaStrings.h"

/************************************************************************/
/*                                                                      */
/*  Layout Constants                                                    */
/*                                                                      */
/************************************************************************/

static const AdaUIRect ASettingsRects[] PROGMEM = {
    { 160, 200, 40, 37 },       // Heat button
    { 201, 200, 40, 37 }        // Cool button
};

static const char* const ASettingsTitles[] PROGMEM = {
    string_time, string_date, NULL, NULL, NULL
};

static const AdaPage ASettings PROGMEM = {
    string_settings, string_back, ASettingsTitles, ASettingsRects, 2
};

/************************************************************************/
/*                                                                      */
/*  Globals                                                             */
/*                                                                      */
/************************************************************************/

static AdaSetDatePage GSetDatePage;
static AdaSetTimePage GSetTimePage;

/************************************************************************/
/*                                                                      */
/*  Class Declaration                                                   */
/*                                                                      */
/************************************************************************/

/*  AdaSettingsPage::AdaSettingsPage
 *
 *      Build me
 */

AdaSettingsPage::AdaSettingsPage() : AdaUIPage(&ASettings)
{  
}

/*  AdaSettingsPage::ViewWillAppear
 *
 *      Determine if the time has changed by looking at the time record.
 *  The time object is the only thing that doesn't directly update the time,
 8  because we reuse it in our schedule code.
 */

void AdaSettingsPage::viewWillAppear()
{
    if (GSetTimePage.changed) {
        AdaTimeRecord tr = AdaSecToTime(AdaGetTime());
        tr.hour = GSetTimePage.hour;
        tr.min = GSetTimePage.minute;
        tr.sec = 0;
        AdaSetTime(AdaTimeToSec(tr));
    }
    
    GSetTimePage.changed = false;
}

/*  AdaSettingsPage::drawContents
 *
 *      Draw the contents
 */

void AdaSettingsPage::drawContents()
{
    char buffer[16];
    
    uint32_t tmp = AdaGetTime();

    GC.setTextColor(ADAUI_RED,ADAUI_BLACK);

    FormatTime(buffer,tmp);
    GC.drawButton(RECT(90,50,160,32),buffer,28,0,KLeftAlign);
    
    FormatDate(buffer,tmp);
    GC.drawButton(RECT(90,88,160,32),buffer,28,0,KLeftAlign);
}

/*  AdaSettingsPage::handleEvent
 *
 *      Handle tap events
 */

void AdaSettingsPage::handleEvent(uint8_t ix)
{
    switch (ix) {
        case AEVENT_LEFTBUTTON1:
            {
                AdaTimeRecord tr = AdaSecToTime(AdaGetTime());
                GSetTimePage.hour = tr.hour;
                GSetTimePage.minute = tr.min;
                GSetTimePage.changed = false;
                pushPage(&GSetTimePage);
                break;
            }
        case AEVENT_LEFTBUTTON2:
            pushPage(&GSetDatePage);
            break;
    }
}

/*  AdaSettingsPage.cpp
 *
 *      Defines the behavior of our home page, which displays the thermostat.
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

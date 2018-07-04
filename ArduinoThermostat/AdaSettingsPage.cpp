/*  AdaSettingsPage.cpp
 *
 *      Defines the behavior of our home page, which displays the thermostat.
 */

#include "AdaUIScreen.h"
#include "AdaSettingsPage.h"
#include "Narrow25.h"
#include "Narrow75D.h"
#include "AdaTime.h"
#include "AdaThermostat.h"
#include "AdaUtils.h"

/************************************************************************/
/*                                                                      */
/*  Layout Constants                                                    */
/*                                                                      */
/************************************************************************/

static const char string_title[] PROGMEM = "SETTINGS";
static const char string_back[] PROGMEM = "\177DONE";

static const char string_time[] PROGMEM = "TIME";
static const char string_date[] PROGMEM = "DATE";

static const AdaUIRect ASettingsRects[] PROGMEM = {
    { 160, 200, 40, 37 },       // Heat button
    { 201, 200, 40, 37 }        // Cool button
};

static const char* const ASettingsTitles[] PROGMEM = {
    string_time, string_date, NULL, NULL, NULL
};

static const AdaPage ASettings PROGMEM = {
    string_title, string_back, ASettingsTitles, ASettingsRects, 2
};

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

/*  AdaSettingsPage::drawContents
 *
 *      Draw the contents
 */

void AdaSettingsPage::drawContents()
{
    char buffer[16];
    
    uint32_t tmp = AdaGetTime();

    GC.setTextColor(ADAUI_RED,ADAUI_BLACK);
    GC.setFont(&Narrow25);

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
}

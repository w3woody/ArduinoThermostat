/*  AdaSchedulePickerPage.cpp
 *
 *      Defines the behavior of our home page, which displays the thermostat.
 */

#include "AdaUIScreen.h"
#include "AdaSchedulePickerPage.h"
#include "AdaSchedulePage.h"
#include "Narrow25D.h"
#include "AdaTime.h"
#include "AdaThermostat.h"
#include "AdaSchedule.h"
#include "AdaUtils.h"
#include "AdaProgmem.h"
#include "AdaStrings.h"

/************************************************************************/
/*                                                                      */
/*  Layout Constants                                                    */
/*                                                                      */
/************************************************************************/

static const AdaUIRect AScheduleRects[] PROGMEM = {
    { 131, 75, 77,37 },
    { 229, 75, 77,37 },
    { 131,113, 77,37 },
    { 229,113, 77,37 },
    { 131,151,175,37 },
};

static const char* const AScheduleTitles[] PROGMEM = {
    NULL, NULL, NULL, NULL, string_edit
};

static const AdaPage ASchedule PROGMEM = {
    string_schedule, string_back, AScheduleTitles, AScheduleRects, 5
};

/************************************************************************/
/*                                                                      */
/*  Globals                                                             */
/*                                                                      */
/************************************************************************/

AdaSchedulePage GSchedulePage;

/************************************************************************/
/*                                                                      */
/*  Class Declaration                                                   */
/*                                                                      */
/************************************************************************/

/*  AdaSchedulePickerPage::AdaSchedulePickerPage
 *
 *      Build me
 */

AdaSchedulePickerPage::AdaSchedulePickerPage() : AdaUIPage(&ASchedule)
{
}

/*  AdaSchedulePickerPage::redrawSelected
 *
 *      Redraw the selected button
 */

void AdaSchedulePickerPage::redrawSelected()
{
    GC.setTextColor(ADAUI_BLACK,GSchedule.getCurSchedule() == 0 ? ADAUI_GREEN : ADAUI_BLACK);
    GC.drawButton(RECT(111, 75,19,37));

    GC.setTextColor(ADAUI_BLACK,GSchedule.getCurSchedule() == 1 ? ADAUI_GREEN : ADAUI_BLACK);
    GC.drawButton(RECT(209, 75,19,37));

    GC.setTextColor(ADAUI_BLACK,GSchedule.getCurSchedule() == 2 ? ADAUI_GREEN : ADAUI_BLACK);
    GC.drawButton(RECT(111,113,19,37));

    GC.setTextColor(ADAUI_BLACK,GSchedule.getCurSchedule() == 3 ? ADAUI_GREEN : ADAUI_BLACK);
    GC.drawButton(RECT(209,113,19,37));

    GC.setTextColor(ADAUI_BLACK,GSchedule.getCurSchedule() == 4 ? ADAUI_GREEN : ADAUI_BLACK);
    GC.drawButton(RECT(111,151,19,37));
}


/*  AdaSchedulePickerPage::drawContents
 *
 *      Draw the contents
 */

void AdaSchedulePickerPage::drawContents()
{
    /*
     *  Draw the button edges
     */
    
    GC.setTextColor(ADAUI_BLACK,ADAUI_BLUE);

    GC.drawButton(RECT(99, 75,11,37),KCornerUL);
    GC.drawButton(RECT(99,113,11,37));
    GC.drawButton(RECT(99,151,11,37),KCornerLL);
    
    /*
     *  Draw the buttons themselves ### Long name for energy saver page?
     */
    
    GC.drawButton(RECT(131, 75, 77,37),(const __FlashStringHelper *)string_spring,28);
    GC.drawButton(RECT(229, 75, 77,37),(const __FlashStringHelper *)string_summer,28,KCornerUR);
    GC.drawButton(RECT(131,113, 77,37),(const __FlashStringHelper *)string_fall,28);
    GC.drawButton(RECT(229,113, 77,37),(const __FlashStringHelper *)string_winter,28);
    GC.drawButton(RECT(131,151,175,37),(const __FlashStringHelper *)string_saver,28,KCornerLR);
    
    /*
     *  Draw the seelcted schedule item
     */
    
    redrawSelected();
}

/*  AdaSchedulePickerPage::handleEvent
 *
 *      Handle tap events
 */

void AdaSchedulePickerPage::handleEvent(uint8_t ix)
{
    switch (ix) {
        case AEVENT_LEFTBUTTON5:
            pushPage(&GSchedulePage);
            break;
            
        case AEVENT_FIRSTSPOT:
        case AEVENT_FIRSTSPOT+1:
        case AEVENT_FIRSTSPOT+2:
        case AEVENT_FIRSTSPOT+3:
        case AEVENT_FIRSTSPOT+4:
            GSchedule.setCurSchedule(ix - AEVENT_FIRSTSPOT);
            redrawSelected();
            break;
    }
}


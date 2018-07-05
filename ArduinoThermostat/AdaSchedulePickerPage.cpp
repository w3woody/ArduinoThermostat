/*  AdaSchedulePickerPage.cpp
 *
 *      Defines the behavior of our home page, which displays the thermostat.
 */

#include "AdaUIScreen.h"
#include "AdaSchedulePickerPage.h"
#include "AdaSchedulePage.h"
#include "Narrow25.h"
#include "Narrow75D.h"
#include "AdaTime.h"
#include "AdaThermostat.h"
#include "AdaSchedule.h"
#include "AdaUtils.h"

#ifdef __AVR__
    #include <avr/pgmspace.h>
#elif defined(ESP8266) || defined(ESP32)
    #include <pgmspace.h>
#endif

/************************************************************************/
/*                                                                      */
/*  Layout Constants                                                    */
/*                                                                      */
/************************************************************************/

static const char string_title[] PROGMEM = "SCHEDULE";
static const char string_back[] PROGMEM = "\177BACK";
static const char string_edit[] PROGMEM = "EDIT";

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
    string_title, string_back, AScheduleTitles, AScheduleRects, 5
};

/************************************************************************/
/*                                                                      */
/*  Globals                                                             */
/*                                                                      */
/************************************************************************/

AdaSchedulePage GSchedulePage;

/************************************************************************/
/*                                                                      */
/*  Support                                                             */
/*                                                                      */
/************************************************************************/

/*
 *  Borrow code from Adafruit_GFX library to handle reading from PROGMEM
 *  space if we are missing some definitions
 */

#if !defined(__INT_MAX__) || (__INT_MAX__ > 0xFFFF)
    #define pgm_read_pointer(addr) ((void *)pgm_read_dword(addr))
#else
    #define pgm_read_pointer(addr) ((void *)pgm_read_word(addr))
#endif

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
    GC.setFont(&Narrow25);
    GC.drawButton(RECT(99, 75,11,37),KCornerUL);
    GC.drawButton(RECT(99,113,11,37));
    GC.drawButton(RECT(99,151,11,37),KCornerLL);
    
    /*
     *  Draw the buttons themselves
     */
    
    GC.drawButton(RECT(131, 75, 77,37),(const __FlashStringHelper *)GStringSpring,28);
    GC.drawButton(RECT(229, 75, 77,37),(const __FlashStringHelper *)GStringSummer,28,KCornerUR);
    GC.drawButton(RECT(131,113, 77,37),(const __FlashStringHelper *)GStringFall,28);
    GC.drawButton(RECT(229,113, 77,37),(const __FlashStringHelper *)GStringWinter,28);
    GC.drawButton(RECT(131,151,175,37),(const __FlashStringHelper *)GStringSaver,28,KCornerLR);
    
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


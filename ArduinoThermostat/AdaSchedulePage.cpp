/*  AdaSchedulePage.cpp
 *
 *      Defines the behavior of our home page, which displays the thermostat.
 */

#include "AdaUIScreen.h"
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
static const char string_back[] PROGMEM = "\177DONE";

static const AdaUIRect AScheduleRects[] PROGMEM = {
    { 160, 200, 40, 37 },       // Heat button
    { 201, 200, 40, 37 }        // Cool button
};

static const AdaPage ASchedule PROGMEM = {
    NULL, string_back, GScheduleName, AScheduleRects, 2
};

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

/*  AdaSchedulePage::AdaSchedulePage
 *
 *      Build me
 */

AdaSchedulePage::AdaSchedulePage() : AdaUIPage(&ASchedule)
{
    selDOW = 0;
    selSchedule = 0;
}

/*  AdaSchedulePage::drawContents
 *
 *      Draw the contents
 */

void AdaSchedulePage::drawContents()
{
    char buffer[8];
    
    GC.setFont(NULL);
    for (uint8_t dow = 0; dow < 7; ++dow) {
        if (dow == selDOW) {
            GC.setTextColor(ADAUI_RED,0x2925);
        } else {
            GC.setTextColor(ADAUI_BLACK,ADAUI_BLUE);
        }
        
        AdaUICorner corner;
        if (dow == 0) corner = KCornerUL;
        else if (dow == 6) corner = KCornerUR | KCornerLR;
        else corner = 0;
        
        GC.drawButton(RECT(100+dow*31,51,30,37),(const __FlashStringHelper *)(GStringDOW + dow*4),26,corner);
    }
    
    /*
     *  Now draw the four settings for this record
     */
    
    AdaScheduleRecord *rec = GSchedule.schedules + GSchedule.getCurSchedule();
    AdaScheduleDay *day = rec->dow + selDOW;
    
    GC.setFont(&Narrow25);
    GC.setTextColor(ADAUI_BLUE,ADAUI_DARKGRAY);

    for (uint8_t ix = 0; ix < 4; ++ix) {
        AdaUICorner cornerLeft = (ix == 3) ? KCornerLL : 0;
        AdaUICorner cornerRight = (ix == 3) ? KCornerLR : 0;

        if (day->setting[ix].hour == 0xFF) {
            GC.drawButton(RECT(100,89+38*ix,31,37),cornerLeft);
            GC.drawButton(RECT(132,89+38*ix,31,37));
            GC.drawButton(RECT(164,89+38*ix,75,37),cornerRight);
        } else {
            FormatNumber(buffer,day->setting[ix].heat);
            GC.drawButton(RECT(100,89+38*ix,31,37),buffer,26,cornerLeft);
            FormatNumber(buffer,day->setting[ix].cool);
            GC.drawButton(RECT(132,89+38*ix,31,37),buffer,26);
            FormatTime(buffer,day->setting[ix].hour,day->setting[0].minute);
            GC.drawButton(RECT(164,89+38*ix,75,37),buffer,26,cornerRight);
        }
    }
    
    GC.setTextColor(ADAUI_BLACK,ADAUI_BLUE);
    GC.drawButton(RECT(245,127,75,37),F("CLEAR"),26,KCornerUL | KCornerUR);
    GC.drawButton(RECT(245,165,75,37),F("COPY"),26);
    GC.drawButton(RECT(245,203,75,37),F("PASTE"),26,KCornerLL | KCornerLR);
}

/*  AdaSchedulePage::drawTitle
 *
 *      Draw the title. This is the name of the currently visible schedule
 */

void AdaSchedulePage::drawTitle()
{
    const __FlashStringHelper *str;
    str = (const __FlashStringHelper *)pgm_read_pointer(GScheduleName+GSchedule.getCurSchedule());
    
    GC.setTextColor(ADAUI_RED,ADAUI_BLACK);
    GC.setFont(&Narrow25);
    GC.drawButton(RECT(160,0,160,32),str,24);
}

/*  AdaSchedulePage::handleTap
 *
 *      Invalidate the contents if we change the DOW
 */

void AdaSchedulePage::handleTap(TS_Point pt)
{
    if ((pt.y < 50) || (pt.y > 88)) return;
    if (pt.x < 100) return;
    
    uint8_t dow = (pt.x - 100) / 31;
    if (dow < 7) {
        if (selDOW != dow) {
            selDOW = dow;
            invalidateContents();
        }
    }
}

/*  AdaSchedulePage::handleEvent
 *
 *      Handle tap events
 */

void AdaSchedulePage::handleEvent(uint8_t ix)
{
    switch (ix) {
        case AEVENT_LEFTBUTTON1:
        case AEVENT_LEFTBUTTON2:
        case AEVENT_LEFTBUTTON3:
        case AEVENT_LEFTBUTTON4:
        case AEVENT_LEFTBUTTON5:
            GSchedule.setCurSchedule(ix);
            invalidateContents();
            drawTitle();
            break;
    }
}

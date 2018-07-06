/*  AdaSchedulePage.cpp
 *
 *      Defines the behavior of our home page, which displays the thermostat.
 */

#include "AdaUIScreen.h"
#include "AdaSchedulePage.h"
#include "Narrow25D.h"
#include "AdaTime.h"
#include "AdaThermostat.h"
#include "AdaSchedule.h"
#include "AdaUtils.h"
#include "AdaSetSchedule.h"
#include "AdaProgmem.h"
#include "AdaStrings.h"

/************************************************************************/
/*                                                                      */
/*  Layout Constants                                                    */
/*                                                                      */
/************************************************************************/

static const AdaUIRect AScheduleRects[] PROGMEM = {
    { 100,  89, 140, 38 },      // Temperature range 1
    { 100, 127, 140, 38 },      // Temperature range 2
    { 100, 165, 140, 38 },      // Temperature range 3
    { 100, 203, 140, 38 },      // Temperature range 4
    { 250, 127,  70, 37 },      // Clear
    { 250, 165,  70, 37 },      // Copy
    { 250, 203,  70, 37 }       // Paste
};

static const AdaPage ASchedule PROGMEM = {
    NULL, string_back, GScheduleNames, AScheduleRects, 7
};

static bool GPasteFlag;             // True if we have something to paste
static AdaScheduleDay GPaste;       // Where we store our stuff

static AdaSetSchedulePage GSchedulePage;    // Page to set schedule

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
}

void AdaSchedulePage::viewWillAppear()
{
    selDOW = 0;
    selSchedule = 0;
    GPasteFlag = false;
    
    if (GSchedulePage.changed) {
        // Change our schedule item
        GSchedulePage.changed = false;
        
        AdaScheduleDay day = GSchedule.getSchedule(selSchedule,selDOW);
        AdaScheduleItem *item = day.setting + selItem;

        item->hour = GSchedulePage.hour;
        item->minute = GSchedulePage.minute;
        item->heat = GSchedulePage.heat;
        item->cool = GSchedulePage.cool;
        
        GSchedule.setSchedule(selSchedule,selDOW,day);
    }
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
        
        GC.drawButton(RECT(100+dow*31,51,30,37),GDOWString(dow),26,corner);
    }
    
    /*
     *  Now draw the four settings for this record
     */
    
    AdaScheduleDay day = GSchedule.getSchedule(selSchedule,selDOW);
    
    GC.setFont(&Narrow25D);
    GC.setTextColor(ADAUI_BLUE,ADAUI_DARKGRAY);

    for (uint8_t ix = 0; ix < 4; ++ix) {
        AdaUICorner cornerLeft = (ix == 3) ? KCornerLL : 0;
        AdaUICorner cornerRight = (ix == 3) ? KCornerLR : 0;

        if (day.setting[ix].hour == 0xFF) {
            GC.drawButton(RECT(100,89+38*ix,31,37),cornerLeft);
            GC.drawButton(RECT(132,89+38*ix,31,37));
            GC.drawButton(RECT(164,89+38*ix,75,37),cornerRight);
        } else {
            FormatNumber(buffer,day.setting[ix].heat);
            GC.drawButton(RECT(100,89+38*ix,31,37),buffer,26,cornerLeft);
            FormatNumber(buffer,day.setting[ix].cool);
            GC.drawButton(RECT(132,89+38*ix,31,37),buffer,26);
            FormatTime(buffer,day.setting[ix].hour,day.setting[ix].minute);
            GC.drawButton(RECT(164,89+38*ix,75,37),buffer,26,cornerRight);
        }
    }
    
    GC.setTextColor(ADAUI_BLACK,ADAUI_BLUE);
    GC.drawButton(RECT(250,127,70,37),(const __FlashStringHelper *)string_clear,26,KCornerUL | KCornerUR);
    GC.drawButton(RECT(250,165,70,37),(const __FlashStringHelper *)string_copy,26);
    
    if (GPasteFlag) {
        GC.setTextColor(ADAUI_BLACK,ADAUI_BLUE);
    } else {
        GC.setTextColor(ADAUI_RED,ADAUI_DARKGRAY);
    }
    GC.drawButton(RECT(250,203,70,37),(const __FlashStringHelper *)string_paste,26,KCornerLL | KCornerLR);
}

/*  AdaSchedulePage::drawTitle
 *
 *      Draw the title. This is the name of the currently visible schedule
 */

void AdaSchedulePage::drawTitle()
{
    // Our color, font was set in the caller
    GC.drawButton(RECT(160,0,160,32),GScheduleString(selSchedule),24);
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
            selSchedule = ix;
            invalidateContents();

            GC.setTextColor(ADAUI_RED,ADAUI_BLACK);
            GC.setFont(&Narrow25D);
            drawTitle();
            break;
            
        case AEVENT_FIRSTSPOT:
        case AEVENT_FIRSTSPOT+1:
        case AEVENT_FIRSTSPOT+2:
        case AEVENT_FIRSTSPOT+3:
            {
                selItem = ix - AEVENT_FIRSTSPOT;
                AdaScheduleDay day = GSchedule.getSchedule(selSchedule,selDOW);
                AdaScheduleItem *item = day.setting + selItem;
                
                GSchedulePage.hour = item->hour;
                GSchedulePage.minute = item->minute;
                GSchedulePage.heat = item->heat;
                GSchedulePage.cool = item->cool;
                pushPage(&GSchedulePage);
            }
            // Open time/temp editor
            break;
            
        case AEVENT_FIRSTSPOT+4:        // Clear
            {
                AdaScheduleDay day = GSchedule.getSchedule(selSchedule,selDOW);
                for (uint8_t i = 0; i < 4; ++i) {
                    day.setting[i].hour = 0xFF;
                }
                GSchedule.setSchedule(selSchedule,selDOW,day);
                invalidateContents();
                break;
            }
        case AEVENT_FIRSTSPOT+5:        // Copy
            {
                GPasteFlag = true;
                GPaste = GSchedule.getSchedule(selSchedule,selDOW);
                invalidateContents();
                break;
            }
        case AEVENT_FIRSTSPOT+6:        // Paste
            {
                if (GPasteFlag) {
                    GSchedule.setSchedule(selSchedule,selDOW,GPaste);
                    invalidateContents();
                }
                break;
            }
    }
}

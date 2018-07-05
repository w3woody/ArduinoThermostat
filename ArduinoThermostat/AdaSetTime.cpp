/*  AdaSetTime.cpp
 *
 *      Defines the behavior of our home page, which displays the thermostat.
 */

#include "AdaUIScreen.h"
#include "AdaSetTime.h"
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

static const char string_title[] PROGMEM = "SET TIME";
static const char string_back[] PROGMEM = "\177DONE";

static const char string_time[] PROGMEM = "TIME";
static const char string_date[] PROGMEM = "DATE";

static const AdaUIRect ATimeRects[] PROGMEM = {
    {  73, 83,40,37 },       // Hour++
    {  73,121,40,37 },       // Hour--
    { 207, 83,40,37 },       // Min++
    { 207,121,40,37 }        // Min--
};

static const AdaPage ATime PROGMEM = {
    string_title, string_back, NULL, ATimeRects, 4
};

/************************************************************************/
/*                                                                      */
/*  Class Declaration                                                   */
/*                                                                      */
/************************************************************************/

/*  AdaSetTimePage::AdaSetTime
 *
 *      Build me
 */

AdaSetTimePage::AdaSetTimePage() : AdaUIPage(&ATime)
{ 
}

void AdaSetTimePage::loadTime()
{
    AdaTimeRecord tr = AdaSecToTime(AdaGetTime());
    hour = tr.hour;
    minute = tr.min;
}

/*  AdaSetTimePage::drawTime
 *
 *      Draw the selected time
 */

void AdaSetTimePage::drawTime()
{
    char buffer[16];
    char *ptr = buffer;
    bool pm;
    uint8_t h = hour;

    pm = h >= 12;
    h %= 12;
    if (h == 0) h = 12;
    
    // Format hour/minute
    if (h >= 10) {
        *ptr++ = '1';
        h %= 10;
    }
    *ptr++ = '0' + h;
    *ptr++ = ':';
    *ptr++ = '0' + (minute / 10);
    *ptr++ = '0' + (minute % 10);
    *ptr++ = ' ';
    if (pm) {
        *ptr++ = 'P';
    } else {
        *ptr++ = 'A';
    }
    *ptr++ = 'M';
    *ptr = 0;
    
    GC.setFont(&Narrow25);
    GC.setTextColor(ADAUI_RED,ADAUI_BLACK);
    GC.drawButton(RECT(113,83,94,75), buffer,45,0,KCenterAlign);
}

void AdaSetTimePage::setTime()
{
    AdaTimeRecord tr = AdaSecToTime(AdaGetTime());
    tr.hour = hour;
    tr.min = minute;
    tr.sec = 0;
    AdaSetTime(AdaTimeToSec(tr));
}

/*  AdaSetTimePage::drawContents
 *
 *      Draw the contents
 */

void AdaSetTimePage::drawContents()
{
    GC.setFont(&Narrow25);
    GC.setTextColor(ADAUI_BLACK,ADAUI_BLUE);
    GC.drawButton(RECT( 73,83,40,37), F("+"),28,KCornerUL | KCornerUR,KCenterAlign);
    GC.drawButton(RECT( 73,121,40,37),F("-"),28,KCornerLL | KCornerLR,KCenterAlign);

    GC.drawButton(RECT(207,83,40,37), F("+"),28,KCornerUL | KCornerUR,KCenterAlign);
    GC.drawButton(RECT(207,121,40,37),F("-"),28,KCornerLL | KCornerLR,KCenterAlign);
    
    drawTime();
}

/*  AdaSetTimePage::handleEvent
 *
 *      Handle tap events
 */

void AdaSetTimePage::handleEvent(uint8_t ix)
{
    switch (ix) {
        case AEVENT_FIRSTSPOT:
            if (hour == 23) {
                hour = 0;
            } else {
                ++hour;
            }
            drawTime();
            setTime();
            break;
        case AEVENT_FIRSTSPOT+1:
            if (hour == 0) {
                hour = 23;
            } else {
                --hour;
            }
            drawTime();
            setTime();
            break;
        case AEVENT_FIRSTSPOT+2:
            if (minute == 59) {
                minute = 0;
            } else {
                ++minute;
            }
            drawTime();
            setTime();
            break;
        case AEVENT_FIRSTSPOT+3:
            if (minute == 0) {
                minute = 59;
            } else {
                --minute;
            }
            drawTime();
            setTime();
            break;
    }
}

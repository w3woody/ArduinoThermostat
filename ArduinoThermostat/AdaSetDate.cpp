/*  AdaSetDatePage.cpp
 *
 *      Defines the behavior of our home page, which displays the thermostat.
 */

#include "AdaUIScreen.h"
#include "AdaSetDate.h"
#include "Narrow25.h"
#include "Narrow75D.h"
#include "AdaTime.h"
#include "AdaThermostat.h"
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

static const char string_title[] PROGMEM = "SET DATE";
static const char string_back[] PROGMEM = "\177DONE";

static const char string_time[] PROGMEM = "TIME";
static const char string_date[] PROGMEM = "DATE";

static const AdaUIRect ADateRects[] PROGMEM = {
    { 10,100,40,37 },       // -Month
    { 51,100,40,37 },       // +Month
    { 10,175,40,37 },       // -Year
    { 51,175,40,37 }        // +Year
};

static const AdaPage ADate PROGMEM = {
    string_title, string_back, NULL, ADateRects, 4
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

/*  AdaSetDatePage::AdaSetDatePage
 *
 *      Build me
 */

AdaSetDatePage::AdaSetDatePage() : AdaUIPage(&ADate)
{  
}

/*  AdaSetDatePage::viewWillAppear
 *
 *      Load date
 */

void AdaSetDatePage::viewWillAppear()
{
    AdaTimeRecord date = AdaSecToTime(AdaGetTime());
    day = date.day;
    month = date.month;
    year = date.year;
}

void AdaSetDatePage::drawMonth()
{
    GC.setFont(&Narrow25);
    GC.setTextColor(ADAUI_RED,ADAUI_BLACK);
    GC.drawButton(RECT( 10,62,81,37),(const __FlashStringHelper *)(GStringMonth + 4 * (month-1)),28,0,KCenterAlign);
}

void AdaSetDatePage::drawYear()
{
    char buffer[8];
    
    FormatNumber(buffer,year);
    GC.setFont(&Narrow25);
    GC.setTextColor(ADAUI_RED,ADAUI_BLACK);
    GC.drawButton(RECT( 10,137,81,37),buffer,28,0,KCenterAlign);
}

/*
 *  ### TODO: 
 *
 *      Set calendar date.
 *      Hit detection in calendar
 */

void AdaSetDatePage::drawCalendar()
{
    char buffer[8];
    uint8_t nmonth = month+1;
    uint16_t nyear = year;
    if (nmonth > 12) {
        nmonth = 1;
        ++nyear;
    }

    // erase content area    
    GC.fillRect(100,50,220,190,ADAUI_BLACK);

    // Draw DOW header in tiny font
    GC.setFont(NULL);
    for (uint8_t dow = 0; dow < 7; ++dow) {
        GC.setTextColor(ADAUI_RED,ADAUI_BLACK);
        GC.setCursor(107+dow*31,50);
        GC.print((const __FlashStringHelper *)(GStringDOW + dow*4));
    }

    // Draw calendar
    
    GC.setFont(&Narrow25);
    
    uint32_t start = AdaGregorianDayCount(1,month,year);
    uint32_t end = AdaGregorianDayCount(1,nmonth,nyear);
    
    mdow = start % 7;            // position of first day
    mdays = end - start;        // # days in the month
    mweeks = (mdays + mdow + 6)/7;   // # visible weeks
    
    mheight = (mweeks <= 5) ? 36 : 30;    // Some months are short.
    
    for (uint8_t i = 0; i < mdays; ++i) {
        FormatNumber(buffer,i+1);
        uint8_t x = (i + mdow) % 7;
        uint8_t y = (i + mdow) / 7;
        
        if (i == day-1) {
            GC.setTextColor(ADAUI_RED,ADAUI_BLACK);
        } else {
            GC.setTextColor(ADAUI_BLUE,ADAUI_DARKGRAY);
        }
        GC.drawButton(RECT(100+x*31,60+y*mheight,30,mheight-1),buffer,24);
    }
}

/*  AdaSetDatePage::drawContents
 *
 *      Draw the contents
 */

void AdaSetDatePage::drawContents()
{
    GC.setFont(&Narrow25);
    GC.setTextColor(ADAUI_BLACK,ADAUI_BLUE);
    GC.drawButton(RECT( 10,100,40,37),F("-"),28,KCornerUL | KCornerLL,KCenterAlign);
    GC.drawButton(RECT( 51,100,40,37),F("+"),28,KCornerUR | KCornerLR,KCenterAlign);
    GC.drawButton(RECT( 10,175,40,37),F("-"),28,KCornerUL | KCornerLL,KCenterAlign);
    GC.drawButton(RECT( 51,175,40,37),F("+"),28,KCornerUR | KCornerLR,KCenterAlign);

    drawMonth();
    drawYear();   
    drawCalendar();
}

void AdaSetDatePage::setDate()
{
    AdaTimeRecord tr = AdaSecToTime(AdaGetTime());
    tr.day = day;
    tr.month = month;
    tr.year = year;
    AdaSetTime(AdaTimeToSec(tr));
}

/*  AdaSetDatePage::handleEvent
 *
 *      Handle tap events
 */

void AdaSetDatePage::handleEvent(uint8_t ix)
{
    switch (ix) {
        case AEVENT_FIRSTSPOT:
            --month;
            if (month < 1) {
                if (year > 2017) --year;
                month = 12;
                drawYear();
            }
            day = 1;
            drawMonth();
            drawCalendar();
            setDate();
            break;
        case AEVENT_FIRSTSPOT+1:
            ++month;
            if (month > 12) {
                ++year;
                month = 1;
                drawYear();
            }
            day = 1;
            drawMonth();
            drawCalendar();
            setDate();
            break;
        case AEVENT_FIRSTSPOT+2:
            if (year > 2017) --year;
            drawYear();
            drawCalendar();
            setDate();
            break;
        case AEVENT_FIRSTSPOT+3:
            ++year;
            drawYear();
            drawCalendar();
            setDate();
            break;
    }
}

void AdaSetDatePage::handleTap(TS_Point pt)
{
    char buffer[8];
    
    for (uint8_t i = 0; i < mdays; ++i) {
        uint8_t x = (i + mdow) % 7;
        uint8_t y = (i + mdow) / 7;
        
        if (PtInRect(pt,RECT(100+x*31,50+y*mheight,31,mheight))) {
            if (day != i + 1) {
                
                /*
                 *  Only draw the two buttons. This reduces flicker while
                 *  picking days
                 */
                
                uint8_t x = (i + mdow) % 7;
                uint8_t y = (i + mdow) / 7;
                GC.setTextColor(ADAUI_RED,ADAUI_BLACK);
                FormatNumber(buffer,i+1);
                GC.drawButton(RECT(100+x*31,60+y*mheight,30,mheight-1),buffer,24);

                x = ((day-1) + mdow) % 7;
                y = ((day-1) + mdow) / 7;
                GC.setTextColor(ADAUI_BLUE,ADAUI_DARKGRAY);
                FormatNumber(buffer,day);
                GC.drawButton(RECT(100+x*31,60+y*mheight,30,mheight-1),buffer,24);

                day = i + 1;
                setDate();
            }
        }
    }
}

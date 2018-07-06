/*  AdaSetDatePage.cpp
 *
 *      Defines the behavior of our home page, which displays the thermostat.
 */

#include "AdaUIScreen.h"
#include "AdaSetDate.h"
#include "Narrow25D.h"
#include "AdaTime.h"
#include "AdaThermostat.h"
#include "AdaUtils.h"
#include "AdaProgmem.h"
#include "AdaStrings.h"

/************************************************************************/
/*                                                                      */
/*  Layout Constants                                                    */
/*                                                                      */
/************************************************************************/

static const AdaUIRect ADateRects[] PROGMEM = {
    { 10,100,40,37 },       // -Month
    { 51,100,40,37 },       // +Month
    { 10,175,40,37 },       // -Year
    { 51,175,40,37 }        // +Year
};

static const AdaPage ADate PROGMEM = {
    string_set_date, string_back, NULL, ADateRects, 4
};

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
    GC.setFont(&Narrow25D);
    GC.setTextColor(ADAUI_RED,ADAUI_BLACK);
    GC.drawButton(RECT( 10,62,81,37),GMonthString(month-1),28,0,KCenterAlign);
}

void AdaSetDatePage::drawYear()
{
    char buffer[8];
    
    FormatNumber(buffer,year);
    GC.setFont(&Narrow25D);
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
        GC.print(GDOWString(dow));
    }

    // Draw calendar
    
    GC.setFont(&Narrow25D);
    
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
    GC.setTextColor(ADAUI_BLACK,ADAUI_BLUE);
    GC.drawButton(RECT( 10,100,40,37),(const __FlashStringHelper *)string_minus,28,KCornerUL | KCornerLL,KCenterAlign);
    GC.drawButton(RECT( 51,100,40,37),(const __FlashStringHelper *)string_plus,28,KCornerUR | KCornerLR,KCenterAlign);
    GC.drawButton(RECT( 10,175,40,37),(const __FlashStringHelper *)string_minus,28,KCornerUL | KCornerLL,KCenterAlign);
    GC.drawButton(RECT( 51,175,40,37),(const __FlashStringHelper *)string_plus,28,KCornerUR | KCornerLR,KCenterAlign);

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

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

/*  AdaSetDatePage::loadDate
 *
 *      Load date
 */

void AdaSetDatePage::loadDate()
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
 *      Draw Calendar should erase prior months.
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

    GC.setFont(&Narrow25);
    
    uint32_t start = AdaGregorianDayCount(1,month,year);
    uint32_t end = AdaGregorianDayCount(1,nmonth,nyear);
    
    uint8_t dow = start % 7;            // position of first day
    uint8_t ndays = end - start;        // # days in the month
    uint8_t nweeks = (ndays + dow + 6)/7;   // # visible weeks
    
    uint8_t h = (nweeks <= 5) ? 38 : 31;    // Some months are short.
    
    for (uint8_t i = 0; i < ndays; ++i) {
        FormatNumber(buffer,i+1);
        uint8_t x = (i + dow) % 7;
        uint8_t y = (i + dow) / 7;
        
        if (i == day-1) {
            GC.setTextColor(ADAUI_RED,ADAUI_BLACK);
        } else {
            GC.setTextColor(ADAUI_BLUE,ADAUI_DARKGRAY);
        }
        GC.drawButton(RECT(100+x*31,50+y*h,30,h-1),buffer,24);
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
            break;
        case AEVENT_FIRSTSPOT+2:
            if (year > 2017) --year;
            drawYear();
            drawCalendar();
            break;
        case AEVENT_FIRSTSPOT+3:
            ++year;
            drawYear();
            drawCalendar();
            break;
    }
}

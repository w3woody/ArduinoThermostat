/*  AdaSetSchedule.cpp
 *
 *      Defines the behavior of our home page, which displays the thermostat.
 */

#include "AdaUIScreen.h"
#include "AdaSetSchedule.h"
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

static const AdaUIRect ATimeRects[] PROGMEM = {
    {  10, 89,40,37 },       // Heat--
    {  51, 89,40,37 },       // Heat++
    { 100, 89,40,37 },       // Cool--
    { 141, 89,40,37 }        // Cool++
};

static const AdaPage ATime PROGMEM = {
    string_schedule, string_back, NULL, ATimeRects, 4
};

/************************************************************************/
/*                                                                      */
/*  Class Declaration                                                   */
/*                                                                      */
/************************************************************************/

/*  AdaSetSchedulePage::AdaSetSchedule
 *
 *      Build me
 */

AdaSetSchedulePage::AdaSetSchedulePage() : AdaSetTimePage(86,&ATime)
{
}

/*  AdaSetSchedulePage::drawContents
 *
 *      Draw the contents
 */

void AdaSetSchedulePage::drawContents()
{
    drawHeat();
    drawCool();
    
    GC.setTextColor(ADAUI_BLACK,ADAUI_BLUE);

    GC.drawButton(RECT( 10, 89,40,37),(const __FlashStringHelper *)string_minus,28,KCornerUL | KCornerLL,KCenterAlign);
    GC.drawButton(RECT( 51, 89,40,37),(const __FlashStringHelper *)string_plus,28,KCornerUR | KCornerLR,KCenterAlign);
    GC.drawButton(RECT(100, 89,40,37),(const __FlashStringHelper *)string_minus,28,KCornerUL | KCornerLL,KCenterAlign);
    GC.drawButton(RECT(141, 89,40,37),(const __FlashStringHelper *)string_plus,28,KCornerUR | KCornerLR,KCenterAlign);

    AdaSetTimePage::drawContents();
}

void AdaSetSchedulePage::drawHeat()
{
    char buffer[8];
    
    FormatNumber(buffer,heat);
    GC.setFont(&Narrow25D);
    GC.setTextColor(ADAUI_RED,ADAUI_BLACK);
    GC.drawButton(RECT(10,50,80,37),buffer,28,0,KCenterAlign);
}

void AdaSetSchedulePage::drawCool()
{
    char buffer[8];
    
    FormatNumber(buffer,cool);
    GC.setFont(&Narrow25D);
    GC.setTextColor(ADAUI_RED,ADAUI_BLACK);
    GC.drawButton(RECT(100,50,80,37),buffer,28,0,KCenterAlign);
}

void AdaSetSchedulePage::handleEvent(uint8_t ix)
{
    switch (ix) {
        case AEVENT_FIRSTSPOT:
            if (heat > 50) {
                --heat;
                changed = true;
                drawHeat();
            }
            break;
        case AEVENT_FIRSTSPOT+1:
            if (heat + 5 < cool) {
                ++heat;
                changed = true;
                drawHeat();
            }
            break;
        case AEVENT_FIRSTSPOT+2:
            if (heat + 5 < cool) {
                --cool;
                changed = true;
                drawCool();
            }
            break;
        case AEVENT_FIRSTSPOT+3:
            if (cool < 90) {
                ++cool;
                changed = true;
                drawCool();
            }
            break;
    }
}

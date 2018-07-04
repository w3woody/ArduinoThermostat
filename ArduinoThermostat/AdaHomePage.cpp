/*  AdaHomePage.cpp
 *
 *      Defines the behavior of our home page, which displays the thermostat.
 */

#include "AdaUIScreen.h"
#include "AdaHomePage.h"
#include "Narrow25.h"
#include "Narrow75D.h"
#include "AdaTime.h"
#include "AdaThermostat.h"
#include "AdaSettingsPage.h"
#include "AdaUtils.h"

#include "AdaTempPage.h"
#include "AdaSchedulePage.h"

/************************************************************************/
/*                                                                      */
/*  Layout Constants                                                    */
/*                                                                      */
/************************************************************************/

static const char string_fan[] PROGMEM = "FAN";
static const char string_schedule[] PROGMEM = "SCHEDULE";
static const char string_settings[] PROGMEM = "SETTINGS";

static const AdaUIRect AHomeRects[] PROGMEM = {
    { 160, 200, 40, 37 },       // Heat button
    { 201, 200, 40, 37 }        // Cool button
};

static const char* const AHomeTitles[] PROGMEM = {
    string_fan, string_schedule, NULL, NULL, string_settings
};

static const AdaPage AHome PROGMEM = {
    NULL, NULL, AHomeTitles, AHomeRects, 2
};

/************************************************************************/
/*                                                                      */
/*  Page Globals                                                        */
/*                                                                      */
/************************************************************************/

static AdaTempPage GTempPage;
static AdaSchedulePage GSchedulePage;
static AdaSettingsPage GSettingsPage;

/************************************************************************/
/*                                                                      */
/*  Drawing Support                                                     */
/*                                                                      */
/************************************************************************/

// Bitmap for ring
const uint8_t Arc_bitmap[] PROGMEM = {
    0x80,0x44,0x99,0x80,0x3E,0xA4,0x80,0x3B,
    0xAB,0x80,0x37,0xB3,0x80,0x34,0xB9,0x80,
    0x32,0xBD,0x80,0x30,0xC1,0x80,0x2E,0xC5,
    0x80,0x2C,0xC9,0x80,0x2A,0xCE,0x80,0x28,
    0xA0,0x11,0xA0,0x80,0x26,0x9C,0x1D,0x9C,
    0x80,0x25,0x99,0x25,0x99,0x80,0x24,0x97,
    0x2B,0x98,0x80,0x22,0x96,0x31,0x96,0x80,
    0x20,0x95,0x37,0x94,0x80,0x1F,0x94,0x3B,
    0x94,0x80,0x1E,0x93,0x3F,0x93,0x80,0x1D,
    0x92,0x43,0x92,0x80,0x1B,0x92,0x47,0x91,
    0x80,0x1A,0x92,0x49,0x91,0x80,0x1A,0x90,
    0x4D,0x91,0x80,0x18,0x91,0x4F,0x91,0x80,
    0x17,0x91,0x52,0x90,0x80,0x16,0x90,0x55,
    0x8F,0x80,0x16,0x8F,0x57,0x8F,0x80,0x15,
    0x8F,0x59,0x90,0x80,0x14,0x8F,0x5B,0x8F,
    0x80,0x13,0x8E,0x5F,0x8E,0x80,0x12,0x8E,
    0x61,0x8E,0x80,0x11,0x8E,0x63,0x8E,0x80,
    0x11,0x8D,0x65,0x8E,0x80,0x10,0x8D,0x66,
    0x8E,0x80,0x0F,0x8D,0x68,0x8E,0x80,0x0E,
    0x8E,0x69,0x8E,0x80,0x0E,0x8D,0x6B,0x8D,
    0x80,0x0D,0x8D,0x6D,0x8D,0x80,0x0D,0x8C,
    0x6F,0x8D,0x80,0x0C,0x8C,0x70,0x8D,0x80,
    0x0B,0x8D,0x71,0x8D,0x80,0x0B,0x8C,0x73,
    0x8C,0x80,0x0A,0x8C,0x75,0x8C,0x80,0x0A,
    0x8C,0x75,0x8C,0x80,0x09,0x8C,0x77,0x8C,
    0x80,0x09,0x8B,0x79,0x8B,0x80,0x08,0x8C,
    0x79,0x8C,0x80,0x08,0x8B,0x7B,0x8B,0x80,
    0x07,0x8C,0x7B,0x8C,0x80,0x07,0x8B,0x7D,
    0x8B,0x80,0x06,0x8C,0x7D,0x8C,0x80,0x06,
    0x8B,0x7E,0x8C,0x80,0x06,0x8B,0x7F,0x8C,
    0x80,0x05,0x8B,0x7F,0x02,0x8B,0x80,0x05,
    0x8B,0x7F,0x02,0x8B,0x80,0x04,0x8C,0x7F,
    0x02,0x8C,0x80,0x04,0x8B,0x7F,0x04,0x8B,
    0x80,0x04,0x8B,0x7F,0x04,0x8B,0x80,0x04,
    0x8B,0x7F,0x04,0x8C,0x80,0x03,0x8B,0x7F,
    0x06,0x8B,0x80,0x03,0x8B,0x7F,0x06,0x8B,
    0x80,0x03,0x8B,0x7F,0x06,0x8B,0x80,0x03,
    0x8A,0x7F,0x08,0x8B,0x80,0x02,0x8B,0x7F,
    0x08,0x8B,0x80,0x02,0x8B,0x7F,0x08,0x8B,
    0x80,0x02,0x8B,0x7F,0x08,0x8B,0x80,0x02,
    0x8A,0x7F,0x0A,0x8A,0x80,0x02,0x8A,0x7F,
    0x0A,0x8A,0x80,0x01,0x8B,0x7F,0x0A,0x8B,
    0x80,0x01,0x8B,0x7F,0x0A,0x8B,0x80,0x01,
    0x8B,0x7F,0x0A,0x8B,0x80,0x01,0x8B,0x7F,
    0x0A,0x8B,0x80,0x01,0x8A,0x7F,0x0C,0x8A,
    0x80,0x01,0x8A,0x7F,0x0C,0x8A,0x80,0x01,
    0x8A,0x7F,0x0C,0x8A,0x80,0x01,0x8A,0x7F,
    0x0C,0x8A,0x80,0x01,0x8A,0x7F,0x0C,0x8A,
    0x80,0x01,0x8A,0x7F,0x0C,0x8A,0x80,0x01,
    0x8A,0x7F,0x0C,0x8A,0x80,0x01,0x8A,0x7F,
    0x0C,0x8A,0x80,0x01,0x8A,0x7F,0x0C,0x8A,
    0x80,0x01,0x8A,0x7F,0x0C,0x8A,0x80,0x01,
    0x8A,0x7F,0x0C,0x8A,0x80,0x01,0x8A,0x7F,
    0x0C,0x8A,0x80,0x01,0x8A,0x7F,0x0C,0x8A,
    0x80,0x01,0x8A,0x7F,0x0C,0x8A,0x80,0x01,
    0x8A,0x7F,0x0C,0x8A,0x80,0x01,0x8A,0x7F,
    0x0C,0x8A,0x80,0x01,0x8A,0x7F,0x0C,0x8A,
    0x80,0x01,0x8B,0x7F,0x0A,0x8B,0x80,0x01,
    0x8B,0x7F,0x0A,0x8B,0x80,0x01,0x8B,0x7F,
    0x0A,0x8B,0x80,0x01,0x8B,0x7F,0x0A,0x8B,
    0x80,0x02,0x8A,0x7F,0x0A,0x8A,0x80,0x02,
    0x8A,0x7F,0x0A,0x8A,0x80,0x02,0x8B,0x7F,
    0x08,0x8B,0x80,0x02,0x8B,0x7F,0x08,0x8B,
    0x80,0x02,0x8B,0x7F,0x08,0x8B,0x80,0x02,
    0x8B,0x7F,0x08,0x8A,0x80,0x03,0x8B,0x7F,
    0x06,0x8B,0x80,0x03,0x8B,0x7F,0x06,0x8B,
    0x80,0x03,0x8B,0x7F,0x06,0x8B,0x80,0x04,
    0x8B,0x7F,0x04,0x8C,0x80,0x04,0x8B,0x7F,
    0x04,0x8B,0x80,0x04,0x8B,0x7F,0x04,0x8B,
    0x80,0x05,0x8B,0x7F,0x02,0x8C,0x80,0x05,
    0x8B,0x7F,0x02,0x8B,0x80,0x05,0x8C,0x7F,
    0x01,0x8B,0x80,0x06,0x8B,0x7F,0x8C,0x80,
    0x06,0x8B,0x7F,0x8B,0x80,0x06,0x8C,0x7D,
    0x8C,0x80,0x07,0x8B,0x7D,0x8B,0x80,0x07,
    0x8C,0x7B,0x8C,0x80,0x08,0x8B,0x7B,0x8B,
    0x80,0x08,0x8C,0x79,0x8C,0x80,0x09,0x8B,
    0x79,0x8B,0x80,0x09,0x8C,0x77,0x8C,0x80,
    0x0A,0x8C,0x75,0x8C,0x80,0x0A,0x8C,0x75,
    0x8C,0x80,0x0B,0x8C,0x73,0x8C,0x80,0x0B,
    0x8D,0x71,0x8D,0x80,0x0C,0x8D,0x70,0x8C,
    0x80,0x0C,0x8D,0x6F,0x8D,0x80,0x0D,0x8D,
    0x6D,0x8D,0x80,0x0E,0x8D,0x6B,0x8D,0x80,
    0x0F,0x8D,0x69,0x8E,0x80,0x0F,0x8E,0x67,
    0x8E,0x80,0x10,0x8D,0x66,0x8E,0x80,0x10,
    0x8E,0x64,0x8F,0x80,0x11,0x8E,0x62,0x8F,
    0x80,0x12,0x8D,0x63,0x8D,0x80,0x13,0x8B,
    0x65,0x8B,0x80,0x14,0x89,0x67,0x8A,0x80,
    0x14,0x88,0x69,0x87,0x80,0x15,0x86,0x6B,
    0x85,0x80,0x17,0x83,0x6D,0x84,0x80,0x17,
    0x82,0x6F,0x82,0x00
};

void DrawArc(uint8_t lastState, int16_t x, int16_t y)
{
    int xc = x;
    int yc = y;
    const uint8_t *p = Arc_bitmap;
    uint8_t c;
    
    // Arc color gives state of HVAC
    uint16_t color;
    if (lastState & ADAHVAC_HEAT) {
        color = 0xF800;
    } else if (lastState & ADAHVAC_COOL) {
        color = 0x001F;
    } else if (lastState & ADAHVAC_FAN) {
        color = ADAUI_YELLOW;
    } else {
        color = ADAUI_GREEN;
    }
    
    GC.startWrite();

    while (0 != (c = pgm_read_byte(p++))) {
        if (c == 0x80) {
            yc++;
            xc = x;
        } else if (c & 0x80) {
            // Horizontal band
            GC.writeFastHLine(xc,yc,0x7F & c,color);
        }
        xc += c & 0x7F;
    }
    
    GC.endWrite();
}

void DrawMarker(uint8_t temp, uint16_t color, bool outside)
{
    int16_t n = temp;
    if (n < 50) n = 50;
    if (n > 90) n = 90;
    double angle = ((3*M_PI/2) * (90 - n))/40 - (M_PI/4);

    double x = cos(angle);
    double y = - sin(angle);

    int16_t x1,y1,x2,y2;
    if (outside) {
        x1 = 200 + x * 75;
        y1 = 150 + y * 75;
        x2 = 200 + x * 85;
        y2 = 150 + y * 85;
    } else {
        x1 = 200 + x * 75;
        y1 = 150 + y * 75;
        x2 = 200 + x * 65;
        y2 = 150 + y * 65;
    }

    GC.drawLine(x1,y1,x2,y2,color);       // white
    GC.fillCircle(x2,y2,5,color);
    
    if (angle > M_PI) {
        x2 -= 29;
        y2 += 18;
    } else if (angle > M_PI/2) {
        x2 -= 29;
        y2 -= 3;
    } else if (angle > 0) {
        x2 += 7;
        y2 -= 3;
    } else {
        x2 += 7;
        y2 += 18;
    }
    
    if (outside) {
        char buffer[8];
        FormatNumber(buffer,temp);
        GC.drawButton(RECT(x2,y2-19,22,24),buffer,19,0,KCenterAlign);
    }
}

void drawTemperatureMarker(uint8_t temp, uint16_t color)
{
    DrawMarker(temp,color,true);
}

void drawTemperature(uint8_t temp)
{
    // Draw the temperature
    char buffer[8];
    FormatNumber(buffer,temp);
    
    GC.setTextColor(0xFFFF,ADAUI_BLACK);
    GC.setFont(&Narrow75D);
    GC.drawButton(RECT(160,100,80,65),buffer,56,0,KCenterAlign);

    // Draw the marker
    DrawMarker(temp,0xFFFF,false);    
}

/************************************************************************/
/*                                                                      */
/*  Construction                                                        */
/*                                                                      */
/************************************************************************/

/*  AdaHomePage::AdaHomePage
 *
 *      Construct me
 */

AdaHomePage::AdaHomePage() : AdaUIPage(&AHome)
{
}

/************************************************************************/
/*                                                                      */
/*  Drawing Support                                                     */
/*                                                                      */
/************************************************************************/

/*  AdaHomePage::drawTitle
 *
 *      Draw the current title. This draws the time in the upper right
 *  corner of our display.
 */

void AdaHomePage::drawTitle()
{
    char buffer[16];

    FormatDayTime(buffer,AdaGetTime());
    GC.setTextColor(ADAUI_RED,ADAUI_BLACK);
    GC.setFont(&Narrow25);
    GC.drawButton(RECT(160,0,160,32),buffer,24);
}

/*  AdaHomePage::drawContents
 *
 *      Draw the contents
 */

void AdaHomePage::drawContents()
{
    char buffer[8];
    
    // Draw temperature arc
    DrawArc(GThermostat.unitState,120,70);

    // Draw temperature adjustment
    GC.setFont(&Narrow25);
    GC.setTextColor(ADAUI_BLUE,ADAUI_DARKGRAY);
    FormatNumber(buffer,GThermostat.heatSetting);
    GC.drawButton(RECT(160,200,40,37),buffer,28,KCornerUL | KCornerLL,KCenterAlign);
    FormatNumber(buffer,GThermostat.coolSetting);
    GC.drawButton(RECT(201,200,40,37),buffer,28,KCornerUR | KCornerLR,KCenterAlign);
    
    // Draw type of display
    GC.setTextColor(ADAUI_PURPLE,ADAUI_BLACK);
    GC.setFont(&Narrow25);
    GC.drawButton(RECT(160,160,80,30),F("WINTER"),26,0,KCenterAlign);
    
    // Draw the temperature
    drawTemperature(GThermostat.curTemperature);

    GC.setTextColor(ADAUI_PURPLE,ADAUI_BLACK);
    GC.setFont(&Narrow25);
    drawTemperatureMarker(lastHeat,0xF800);       // red
    drawTemperatureMarker(lastCool,0x001F);       // blue
}

/*  AdaHomePage::handleEvent
 *
 *      Scan to see if the time changed, if the temperature changed, and what
 *  state the thermostat is inline. Also handle hit detection of my hand-drawn
 */

void AdaHomePage::handleEvent(uint8_t ix)
{
    switch (ix) {
        case AEVENT_LEFTBUTTON1:
        case AEVENT_FIRSTSPOT:
        case AEVENT_FIRSTSPOT+1:
            pushPage(&GTempPage);
            break;
        case AEVENT_LEFTBUTTON2:
            pushPage(&GSchedulePage);
            break;
        case AEVENT_LEFTBUTTON5:
            pushPage(&GSettingsPage);
            break;
    }
}

void AdaHomePage::periodicEvents()
{
    /*
     *  Check and redraw the time. Note we throttle to only redraw every
     *  minute rather than once a second.
     */
    
    uint8_t t = AdaGetTime() / 60;
    if (drewTime != t) {
        drewTime = t;
        drawTitle();
    }

    /*
     *  Determine the last state we draw. If it has changed, ask for a
     *  redraw. We just redraw the whole display, and yes I know this
     *  causes our world to flicker, but I assume this is a rare event.
     */
    
    if ((GThermostat.heatSetting != lastHeat) || (GThermostat.coolSetting != lastCool) ||
        (GThermostat.curTemperature != lastTemp) || (GThermostat.unitState != lastState)) {
        
        lastHeat = GThermostat.heatSetting;
        lastCool = GThermostat.coolSetting;
        lastTemp = GThermostat.curTemperature;
        lastState = GThermostat.unitState;
        
        invalidateContents();
    }
}

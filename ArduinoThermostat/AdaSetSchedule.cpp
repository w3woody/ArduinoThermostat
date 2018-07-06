/*  AdaSetSchedule.cpp
 *
 *      Defines the behavior of our home page, which displays the thermostat.
 */

#include "AdaUIScreen.h"
#include "AdaSetSchedule.h"
#include "Narrow25.h"
#include "Narrow75D.h"
#include "AdaTime.h"
#include "AdaThermostat.h"
#include "AdaUtils.h"

/************************************************************************/
/*                                                                      */
/*  In memory                                                           */
/*                                                                      */
/************************************************************************/

#ifdef __AVR__
    #include <avr/pgmspace.h>
#elif defined(ESP8266) || defined(ESP32)
    #include <pgmspace.h>
#endif

#ifndef pgm_read_byte
    #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif

/************************************************************************/
/*                                                                      */
/*  Layout Constants                                                    */
/*                                                                      */
/************************************************************************/

static const char string_title[] PROGMEM = "SET SCHEDULE";
static const char string_back[] PROGMEM = "\177DONE";

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
    AdaSetTimePage::drawContents();
}

void AdaSetSchedulePage::drawHeat()
{
}

void AdaSetSchedulePage::drawCool()
{
}



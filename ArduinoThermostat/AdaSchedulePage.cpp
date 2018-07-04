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

/************************************************************************/
/*                                                                      */
/*  Layout Constants                                                    */
/*                                                                      */
/************************************************************************/

static const char string_title[] PROGMEM = "SCHEDULE";
static const char string_back[] PROGMEM = "\177DONE";
static const char string_edit[] PROGMEM = "EDIT";

static const AdaUIRect AScheduleRects[] PROGMEM = {
    { 160, 200, 40, 37 },       // Heat button
    { 201, 200, 40, 37 }        // Cool button
};

static const char* const AScheduleTitles[] PROGMEM = {
    string_edit, NULL, NULL, NULL, NULL
};

static const AdaPage ASchedule PROGMEM = {
    string_title, string_back, AScheduleTitles, AScheduleRects, 2
};

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

/*  AdaSchedulePage::drawContents
 *
 *      Draw the contents
 */

void AdaSchedulePage::drawContents()
{
}

/*  AdaSchedulePage::handleEvent
 *
 *      Handle tap events
 */

void AdaSchedulePage::handleEvent(uint8_t ix)
{
}

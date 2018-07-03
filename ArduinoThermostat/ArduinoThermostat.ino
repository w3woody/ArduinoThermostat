/*  ArduinoThermostat
 *   
 *      This is a demo of UI design on an Arduino with a simple programmable 
 *  thermostat. This lacks the back-end code to actually control an HVAC unit, 
 *  but adding that code should be relatively simple.
 *  
 *      This is the source code that accommodates a series of articles on 
 *  The Hacking Den on User Interface Design, at 
 *  https://hackingden.com/category/user-interface-design/ 
 *  
 *  
 *  Copyright © 2018 by William Edward Woody
 *
 *  This program is free software: you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation, either version 3 of the License, or 
 *  (at your option) any later version.
 */


/*
 *  Much of our machinery has been moved elsewhere.
 *
 *  1.  The code for setting up the screen is contained in AdaUIScreen.cpp/h
 *  2.  The code for hit detection and managing the screens are in 
 *      AdaUIPage.cpp/h
 *  3.  The first screen we show, the home page, is contained in AdaHomePage.
 *      This also contains the declarations of the subsequent screens shown
 *
 *  That implies there is very little for this to do but call the initializer,
 *  start the page display mechanism, and poll for changes in time or temp.
 */

#include "AdaUIScreen.h"
#include "AdaUIPage.h"
#include "AdaHomePage.h"


// const char string_fan[] PROGMEM = "FAN";
// const char string_schedule[] PROGMEM = "SCHEDULE";
// const char string_settings[] PROGMEM = "SETTINGS";
// const char string_title[] PROGMEM = "TITLE";
// const char string_back[] PROGMEM = "\177BACK";
// 
// const char* const AHomeTitles[] PROGMEM = {
//     string_fan, string_schedule, NULL, NULL, string_settings
// };
// 
// const AdaPage AHome PROGMEM = {
//     string_title, string_back, AHomeTitles
// };

AdaHomePage AHomePage;

void setup() 
{
    /*
     *  Start screen
     */
    
    AInitScreen();

    AdaUIPage::pushPage(&AHomePage);
}

void loop() 
{
    AdaUIPage::processEvents();
}

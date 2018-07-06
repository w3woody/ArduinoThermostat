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
 *  
 *  ArduinoThermostat copyright © 2018 by William Edward Woody
 *  
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the Free 
 *  Software Foundation, either version 3 of the License, or (at your option) 
 *  any later version.
 *  
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for 
 *  more details.
 *  
 *  You should have received a copy of the GNU General Public License with this
 *  source distribution. If you did not, you may download a copy of the GNU
 *  General Public License at http://www.gnu.org/licenses/
 *  
 *  If you wish for a commercial license which does not require the
 *  distribution of this source code, or if you wish support with modification
 *  of this code, please contact:
 *  
 *  William Edward Woody
 *  12605 Raven Ridge Rd
 *  Raleigh, NC 27614
 *  United States of America
 *  woody@alumni.caltech.edu
 *
 *
 *  The original sources for this Arduino application may be downloaded from
 *  https://github.com/w3woody/ArduinoThermostat
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
#include "AdaTime.h"
#include "AdaThermostat.h"
#include "AdaSchedule.h"

/*
 *  Our home page.
 */

AdaHomePage AHomePage;

void setup() 
{
    /*
     *  Start screen
     */
    
    AInitScreen();
    AdaTimeInitialize();        /* Start clock */

    AdaUIPage::pushPage(&AHomePage);
}

void loop() 
{
    // Process schedule update
    GSchedule.periodicUpdate();

    // Process thermostat state
    GThermostat.periodicUpdate();
    
    // Process periodic UI events
    AdaUIPage::processEvents();
}

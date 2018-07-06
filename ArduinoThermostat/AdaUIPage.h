/*  AdaUIPage.h
 *
 *      Defines the base class for managing the individual pages on our UI.
 *  This provides common definitions for drawing the inverted L page, for
 *  placing buttons on the page and for hit detection. This also includes
 *  inheritable methods for drawing custom stuff.
 *
 *      Our model is that we display one page at a time, and our class
 *  contains globals which control that state. Ideally our loop() method
 *  would simply periodically call into this class to track and manage the
 *  current pages on display.
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

#ifndef _AdaUIPage_h
#define _AdaUIPage_h

#include "AdaUI.h"
#include "AdaUIScreen.h"
#include "AdaUtils.h"

/************************************************************************/
/*                                                                      */
/*  Internal Constants                                                  */
/*                                                                      */
/************************************************************************/

#define INVALIDATE_DRAW         0x01
#define INVALIDATE_CONTENT      0x02

/************************************************************************/
/*                                                                      */
/*  Event IDs                                                           */
/*                                                                      */
/************************************************************************/

#define AEVENT_LEFTBUTTON1      0
#define AEVENT_LEFTBUTTON2      1
#define AEVENT_LEFTBUTTON3      2
#define AEVENT_LEFTBUTTON4      3
#define AEVENT_LEFTBUTTON5      4

#define AEVENT_FIRSTSPOT        16      /* First hit spot in hit spots array */

/************************************************************************/
/*                                                                      */
/*  Layout Structures                                                   */
/*                                                                      */
/************************************************************************/

/*  AdaPage
 *
 *      This contains three things: an array of titles to use on the left
 *  hand side of the content area, the title of the page (nil if this
 *  is dynamic), and a list of button locations
 */

struct AdaPage
{
    const char *title;
    const char *back;           // back string
    const char **list;          // List of five left buttons or NULL
    AdaUIRect  *hitSpots;       // Hit detection spots or NULL
    uint8_t    hitCount;        // # of hit detection spots
};

/************************************************************************/
/*                                                                      */
/*  Settings                                                            */
/*                                                                      */
/************************************************************************/

/*  AdaUIPage
 *
 *      UI Page
 */

class AdaUIPage
{
    public:
                        AdaUIPage(const AdaPage *p);
                        
        /*
         *  Global management
         */
        
        static void     pushPage(AdaUIPage *page);
        void            popPage();
        
        static void     processEvents();

        /*
         *  Page management
         */
        
        void            draw();
        
        void            invalidate()
                            {
                                invalidFlags |= INVALIDATE_DRAW;
                            }
        void            invalidateContents()
                            {
                                invalidFlags |= INVALIDATE_CONTENT;
                            }

        virtual void    handleTap(TS_Point pt); // Allow other tap handling
        virtual void    handleEvent(uint8_t ix); // Hit Index
        
        virtual void    periodicEvents();   // Called periodically to run events
        
        virtual void    drawTitle();        // Override if custom title        
        virtual void    drawContents();     // Override for custom contents
        
        virtual void    viewWillAppear();
        virtual void    viewWillDisappear();
    protected:
        void            processPageEvents();
        
        /*
         *  Linked list of visible pages.
         */
        
        AdaUIPage       *next;
        static AdaUIPage *top;
        
    private:
        /*
         *  Page contents
         */
        
        bool            wideScreen;         // content area wide
        uint8_t         invalidFlags;       // Invalid flag regions
        const AdaPage   *page;              // Must be in program memory
        
        /*
         *  Tap
         */
        
        bool            lastDown;
};

#endif

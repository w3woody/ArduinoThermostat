/*  AdaUIPage.cpp
 *
 *      Defines the base class for managing the individual pages on our UI.
 *  This provides common definitions for drawing the inverted L page, for
 *  placing buttons on the page and for hit detection. This also includes
 *  inheritable methods for drawing custom stuff.
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

#include "AdaUIPage.h"
#include "Narrow25D.h"
#include "AdaProgmem.h"

/************************************************************************/
/*                                                                      */
/*  Layout Constants                                                    */
/*                                                                      */
/************************************************************************/

// Constants assume 240 pixel tall display. Note we draw a blank above
// each button if defined.

#define NUMBUTTONS          5       // Buttons on left
#define TOPBARBOTTOM        50      // Bottom of top bar area.
#define LEFTBUTTONHEIGHT    38      // (240-50)/5 = 38

/************************************************************************/
/*                                                                      */
/*  Globals                                                             */
/*                                                                      */
/************************************************************************/

AdaUIPage *AdaUIPage::top;


/************************************************************************/
/*                                                                      */
/*  Program Memory Support                                              */
/*                                                                      */
/************************************************************************/

/*  AdaUIPage::AdaUIPage
 *
 *      Construction
 */

AdaUIPage::AdaUIPage(const AdaPage *p)
{
    page = p;
    invalidFlags = 0xFF;        // Set all flags
}

/************************************************************************/
/*                                                                      */
/*  Page Management                                                     */
/*                                                                      */
/************************************************************************/

/*  AdaUIPage::pushPage
 *
 *      Add page to our system
 */

void AdaUIPage::pushPage(AdaUIPage *page)
{
    if (top) top->viewWillDisappear();
    if (page) page->viewWillAppear();
    
    page->next = top;
    top = page;
    
    top->invalidFlags = 0xFF;    // Force top page to redraw
}

void AdaUIPage::popPage()
{
    if (next) {
        viewWillDisappear();
        next->viewWillAppear();
        
        top = next;
        top->invalidFlags = 0xFF; // Force redraw of top page.
    }
}

/*  processPageEvents
 *
 *      Process events on my page
 */

void AdaUIPage::processPageEvents()
{
    const char *tmp;
    
    /*
     *  Process events
     */
     
    periodicEvents();
    
    /*
     *  Redraw if necessary
     */
    
    if (invalidFlags) {
        GC.setFont(&Narrow25D);                     // Our guarantee about font

        if (invalidFlags & INVALIDATE_DRAW) {
            draw();
        } else if (invalidFlags & INVALIDATE_CONTENT) {
            // Erase content area
            if (wideScreen) {
                GC.fillRect(0,40,320,200,0x0000);       // Fill with black
            } else {
                GC.fillRect(95,40,225,200,0x0000);      // Fill with black
            }
            
            drawContents();
        }
        invalidFlags = 0;
    }
    
    /*
     *  Now test touch events. Note we use 'lastDown' to determine if we've
     *  detected a tap already. If we have detected one, then we skip
     *  processing.
     */

    if (Touch.touched()) {
        if (!lastDown) {
            lastDown = true;

            TS_Point p = Touch.getPoint();
        
            /*
             *  The touch coordinate is not rotated, even though we rotate our
             *  screen. So adjust the coordinates
             */
        
            int16_t tmp = p.x;
            p.x = 319 - p.y;
            p.y = tmp;
                
            /*
             *  Back
             */
        
            tmp = (const char *)pgm_read_pointer(&page->back);
            if (tmp && PtInRect(p,RECT(0,0,80,40))) {
                popPage();
                return;
            }
        
            /*
             *  Left buttons
             */

            const char **leftArray = (const char **)pgm_read_pointer(&page->list);
            if (leftArray != NULL) {
                for (uint8_t i = 0; i < NUMBUTTONS; ++i) {
                    tmp = (const char *)pgm_read_pointer(leftArray+i);
                    if (tmp && PtInRect(p,RECT(0,(TOPBARBOTTOM+1)+LEFTBUTTONHEIGHT*i,
                                           80,LEFTBUTTONHEIGHT-1))) {
                        handleEvent(i);
                        return;
                    }
                }
            }
        
            /*
             *  Other buttons
             */
        
            AdaUIRect *llist = pgm_read_pointer(&page->hitSpots);
            uint8_t len = (uint8_t)pgm_read_pointer(&page->hitCount);
            for (uint8_t i = 0; i < len; ++i) {
                AdaUIRect r;
                r.x = pgm_read_word(&(llist[i].x));
                r.y = pgm_read_word(&(llist[i].y));
                r.w = pgm_read_byte(&(llist[i].w));
                r.h = pgm_read_byte(&(llist[i].h));
                if (PtInRect(p,r)) {
                    handleEvent(i+AEVENT_FIRSTSPOT);
                    return;
                }
            }
            
            /*
             *  Try another way
             */
            
            handleTap(p);
        }
    } else {
        /*
         *  Tap is up.
         */
        
        lastDown = false;
    }
}

/*  processEvents
 *
 *      See if there is anything to do, and do it.
 */

void AdaUIPage::processEvents()
{
    if (top == NULL) return;        // ???
    top->processPageEvents();
}

/************************************************************************/
/*                                                                      */
/*  Page Drawing Support                                                */
/*                                                                      */
/************************************************************************/

/*  AdaUIPage::draw
 *
 *      Draw the contents of the page. This runs through and draws all of
 *  the buttons, the title and the rest
 *
 *      ### TODO: Implement invalidFlags
 */

void AdaUIPage::draw()
{
    /*
     *  Clear and set up. Note that this supports our guarantee that we are
     *  setting the font to Narrow25D for all our pages, so they don't have
     *  to unless they're changing the font.
     */
    
    GC.fillScreen(ADAUI_BLACK);
    
    /*
     *  If we have a built-in title, draw that. Otherwise, call drawTitle
     */
    
    GC.setTextColor(ADAUI_RED,ADAUI_BLACK);
    const char *title = (const char *)pgm_read_pointer(&page->title);
    if (title == NULL) {
        // We guarantee the correct font and color set for our title.
        GC.setTextColor(ADAUI_RED,ADAUI_BLACK);
        drawTitle();
    } else {
        GC.drawButton(RECT(160,0,160,32),(const __FlashStringHelper *)title,24);
    }
    
    /*
     *  Draw back button
     */
    
    GC.setFont(&Narrow25D);
    GC.setTextColor(ADAUI_BLUE,ADAUI_BLACK);
    title = (const char *)pgm_read_pointer(&page->back);
    if (title != NULL) {
        GC.drawButton(RECT(0,0,80,32),(const __FlashStringHelper *)title,24,0,KLeftAlign);
    }
    
    /*
     *  If we have any left items, draw the bar
     */
    
    GC.setTextColor(ADAUI_BLACK,ADAUI_RED);

    const char **leftArray = (const char **)pgm_read_pointer(&page->list);
    if (leftArray == NULL) {
        wideScreen = true;
        
        GC.fillRect(0,32,320,7,ADAUI_RED);  // Top bar w/o left
    } else {
        wideScreen = false;
        
        GC.drawTopBar();
        
        /*
         *  Draw left buttons
         */
        
        uint8_t i = 0;
        while (i < NUMBUTTONS) {
            title = (const char *)pgm_read_pointer(leftArray+i);
            if (title) {
                GC.setTextColor(ADAUI_BLACK,ADAUI_BLUE);
                GC.drawButton(RECT(0,(TOPBARBOTTOM+1)+LEFTBUTTONHEIGHT*i,
                                   80,LEFTBUTTONHEIGHT-1),
                              (const __FlashStringHelper *)title,30);
                ++i;
            } else {
                uint8_t j = i;
                while (j < 5) {
                    title = (const char *)pgm_read_pointer(leftArray+j);
                    if (title) break;
                    ++j;
                }
                
                /*
                 *  At this point j points past the last thing to be drawn.
                 *  Draw a red block to fill
                 */
                
                AdaUIRect r;
                
                r.x = 0;
                r.w = 80;
                r.y = TOPBARBOTTOM+LEFTBUTTONHEIGHT*i;
                r.h = LEFTBUTTONHEIGHT*(j-i);
                if (i > 0) {
                    r.y++;
                    r.h--;
                }
                
                GC.setTextColor(ADAUI_BLACK,ADAUI_RED);
                GC.drawButton(r);
                i = j;
            }
        }
    }
    
    /*
     *  Now draw the contents.
     */
    
    drawContents();
}

/*  AdaUIPage:;drawTitle
 *
 *      Draw title. If overridden, this takes full responsibility for
 *  positioning and drawing the title.
 */

void AdaUIPage::drawTitle()
{
}       

/*  AdaUIPage:;drawContents
 *
 *      Draw contents
 */

void AdaUIPage::drawContents()
{
}

/*  AdaUIPage::handleEvent
 *
 *      Default does nothing
 */

void AdaUIPage::handleEvent(uint8_t ix)
{
}

void AdaUIPage::periodicEvents()
{
}

void AdaUIPage::viewWillAppear()
{
}

void AdaUIPage::viewWillDisappear()
{
}

void AdaUIPage::handleTap(TS_Point pt)
{
}

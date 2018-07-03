/*  AdaUIPage.cpp
 *
 *      Defines the base class for managing the individual pages on our UI.
 *  This provides common definitions for drawing the inverted L page, for
 *  placing buttons on the page and for hit detection. This also includes
 *  inheritable methods for drawing custom stuff.
 */

#include "AdaUIPage.h"
#include "AdaUIScreen.h"
#include "Narrow25.h"

#ifdef __AVR__
    #include <avr/pgmspace.h>
#elif defined(ESP8266) || defined(ESP32)
    #include <pgmspace.h>
#endif

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
/*  Program Memory Support                                              */
/*                                                                      */
/************************************************************************/

/*
 *  Borrow code from Adafruit_GFX library to handle reading from PROGMEM
 *  space if we are missing some definitions
 */

#ifndef pgm_read_byte
    #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif

#ifndef pgm_read_word
    #define pgm_read_word(addr) (*(const unsigned short *)(addr))
#endif

#if !defined(__INT_MAX__) || (__INT_MAX__ > 0xFFFF)
    #define pgm_read_pointer(addr) ((void *)pgm_read_dword(addr))
#else
    #define pgm_read_pointer(addr) ((void *)pgm_read_word(addr))
#endif

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
    if (top) top->pageWillHide();
    page->pageWillShow();
    
    page->next = top;
    top = page;
    
    top->invalidFlags = 0xFF;    // Force top page to redraw
}

void AdaUIPage::popPage()
{
    if (next) {
        pageWillHide();
        next->pageWillShow();
    
        top = next;
        top->invalidFlags = 0xFF; // Force redraw of top page.
    }
}

/*  processEvents
 *
 *      See if there is anything to do, and do it.
 */

void AdaUIPage::processEvents()
{
    if (top == NULL) return;        // ???
    
    /*
     *  Redraw if necessary
     */
    
    if (top->invalidFlags) {
        top->draw();
        top->invalidFlags = 0;
    }
    
    /*
     *  Now test touch events
     */

    if (Touch.touched()) {
        TS_Point p = Touch.getPoint();
        // Hit detection TODO
    }
    
    /*
     *  Finally process events
     */
     
    top->periodicEvents();
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
     *  Clear and set up
     */
    
    GC.fillScreen(ADAUI_BLACK);
    GC.setFont(&Narrow25);
    
    /*
     *  If we have a built-in title, draw that. Otherwise, call drawTitle
     */
    
    GC.setTextColor(ADAUI_RED,ADAUI_BLACK);
    const char *title = (const char *)pgm_read_pointer(&page->title);
    if (title == NULL) {
        drawTitle();
    } else {
        GC.drawButton(RECT(160,0,160,32),(const __FlashStringHelper *)title,24);
    }
    
    /*
     *  Draw back button
     */
    
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
        GC.fillRect(0,TOPBARBOTTOM,320,7,ADAUI_RED);
    } else {
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
                
                GC.setTextColor(ADAUI_BLACK,ADAUI_RED);
                GC.drawButton(RECT(0,(TOPBARBOTTOM+1)+LEFTBUTTONHEIGHT*i,
                                   80,LEFTBUTTONHEIGHT*(j-i)-1));
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

/*  AdaUIPage::pageWillShow
 *
 *      Default does nothing
 */

void AdaUIPage::pageWillShow()
{
}

/*  AdaUIPage::pageWillHide
 *
 *      Default does nothing
 */

void AdaUIPage::pageWillHide()
{
}

/*  AdaUIPage::handleEvent
 *
 *      Default does nothing
 */

void AdaUIPage::handleEvent(int8_t ix)
{
}

void AdaUIPage::periodicEvents()
{
}

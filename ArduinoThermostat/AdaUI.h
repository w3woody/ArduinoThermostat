/*  AdaUI.h
 *
 *      Extends the Adafruit GFX library to provide our basic UI elements
 *  and flicker-free redrawing of text.
 */
 
#ifndef _AdaUI_h
#define _AdaUI_h

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ILI9341.h>

/************************************************************************/
/*                                                                      */
/*  Settings                                                            */
/*                                                                      */
/************************************************************************/

//#define ADAUI_NOBLINK           1

/************************************************************************/
/*                                                                      */
/*  Constants                                                           */
/*                                                                      */
/************************************************************************/

enum AdaUIBarOrientation
{
    KBarOrientUL = 0,       // Curves to the upper-left
    KBarOrientLL,           // Curves to the lower-left (inverted L, default)
    KBarOrientUR,           // Curves to the upper-right
    KBarOrientLR,           // Curves to the lower-right
};

enum AdaUICorner
{
    KCornerUL = 1,
    KCornerLL = 2,
    KCornerUR = 4,
    KCornerLR = 8
};

enum AdaUIAlignment
{
    KLeftAlign = 1,
    KCenterAlign,
    KRightAlign
};

/************************************************************************/
/*                                                                      */
/*  Standard Colors                                                     */
/*                                                                      */
/************************************************************************/

/*
 *  Note that our colors are defined in the RGB color space of the ILI9341
 *  device, which is 16-bit RGB, 565.
 */
 
#define ADAUI_RED       0xC945      // H=0, S=80%, V=80%
#define ADAUI_GREEN     0x664C      // H=120, S=50%, V=80%
#define ADAUI_BLUE      0x7BD9      // H=240, S=40%, V=80%
#define ADAUI_YELLOW    0xFD40      // H=40, S=100%, V=100%
#define ADAUI_PURPLE    0xCCD9      // H=300, S=25%, V=80%
#define ADAUI_DARKGRAY  0x2925      // H=0, S=0%, V=15%
#define ADAUI_BLACK     0x0000      // Black

/************************************************************************/
/*                                                                      */
/*  Structures                                                          */
/*                                                                      */
/************************************************************************/

struct AdaUIRect
{
    int16_t x;
    int16_t y;
    uint8_t w;
    uint8_t h;
};

#define RECT(x,y,w,h) ((AdaUIRect){ x, y, w, h })

/************************************************************************/
/*                                                                      */
/*  Class Declarations                                                  */
/*                                                                      */
/************************************************************************/

/*  AdaUI
 *
 *      Technically this inherits from the ILI9314 library, which provides
 *  the functionality for drawing on the specific board in our sample code.
 *  However, this code could inherit from any child of Adafruit_GFX, and
 *  in fact, we could in theory fork the Adafruit_GFX library and add our
 *  code directly there. (We don't, because that opens its own can of worms.)
 *
 *      One advantage of inheriting like this is that we get access to the
 *  private interfaces and classes and we can use the state that is set
 *  in the UI.
 */
 
class AdaUI: public Adafruit_ILI9341
{
    public:
        /*
         *  Expose our ILI9314 constructors by redirecting to them.
         */
         
        AdaUI(int8_t _CS, int8_t _DC, int8_t _MOSI, int8_t _SCLK, int8_t _RST = -1, int8_t _MISO = -1) : Adafruit_ILI9341(_CS,_DC,_MOSI,_SCLK,_RST,_MISO)
            {
            }
        AdaUI(int8_t _CS, int8_t _DC, int8_t _RST = -1) : Adafruit_ILI9341(_CS,_DC,_RST)
            {
            }
            
        
        /*
         *  Our custom UI elements. We only have two: the rounded separator
         *  bar at top, and the rounded rect with right-aligned text.
         */
        
        /*  drawTopBar
         *
         *      Draw the curved element in the area provided. This takes five
         *  parameters: the position, width and width of the side bar, along
         *  with the orientation of the curve (upper-left, lower-left,
         *  upper-right,lower-right).
         *
         *      If called without parameters, draws our curve at the default
         *  inverted-L location
         */
        
        void drawTopBar(int16_t top = 32, AdaUIBarOrientation orient = KBarOrientLL, 
                        int16_t left = 0, int16_t width = 0, int16_t lbarwidth = 80);
        
        /*  drawButton
         *
         *      The only other custom element in our custom UI is our 
         *  button/rectangle area. This is at a given rectangular area, with
         *  text that can be left, center or right-aligned, and with 
         *  rounded corners. The radius is fixed.
         */
        
        void drawButton(AdaUIRect r, AdaUICorner corners = 0);
                        
        void drawButton(AdaUIRect r, const __FlashStringHelper *str, 
                        int16_t baseline, AdaUICorner corners = 0,
                        AdaUIAlignment align = KRightAlign);
                       
        void drawButton(AdaUIRect r, const char *str, int16_t baseline, 
                        AdaUICorner corners = 0,
                        AdaUIAlignment align = KRightAlign);
                        
    protected:
        int16_t charWidth(uint8_t ch);
        int16_t stringWidth(const char *str);
        int16_t stringWidth(const __FlashStringHelper *str);
    
#if ADAUI_NOBLINK == 1
        void drawButtonInternal(AdaUIRect r, void *str, bool inPgm, 
                        int16_t baseline, AdaUICorner corners, 
                        AdaUIAlignment align);
                        
        
        void drawButton(AdaUIRect rect, AdaUICorner corners, int16_t l, int16_t r);
#endif
};

#endif

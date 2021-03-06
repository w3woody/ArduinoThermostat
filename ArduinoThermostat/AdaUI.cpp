/*  AdaUI
 *
 *      This provides the various custom UI drawing routines for our
 *  custom UI.
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

#include "AdaUI.h"
#include "AdaProgmem.h"

/************************************************************************/
/*                                                                      */
/*  Built in curves                                                     */
/*                                                                      */
/************************************************************************/

const int8_t Curve18[] PROGMEM = {
     3,  6,  8, 10, 11, 12, 13, 14, 14, 
    15, 15, 16, 16, 16, 17, 17, 17, 17
};

const int8_t Curve11[] PROGMEM = {
     2,  4,  6,  7,  8,  8,
     9,  9, 10, 10, 10
};

/************************************************************************/
/*                                                                      */
/*  Top bar drawing                                                     */
/*                                                                      */
/************************************************************************/

/*  AdaUI::drawTopBar
 *
 *      Draw the top bar. Uses the text color and background colors to
 *  define the color of the button
 */

void AdaUI::drawTopBar(int16_t top, AdaUIBarOrientation orient,
        int16_t left, int16_t width, int16_t lbarwidth)
{
    int8_t tmp;
    int16_t xl,xw;
    
    startWrite();
    if (width == 0) {
        width = _width - left;  // Draw to far right if zero
    }
    
    for (uint8_t y = 0; y < 18; ++y) {
        switch (orient) {
            default:
            case KBarOrientLL:
                tmp = pgm_read_byte(Curve18 + y);
                xl = left + 17 - tmp;
                if (y < 7) {
                    xw = width - 17 + tmp;
                } else {
                    xw = left + lbarwidth + 10 - pgm_read_byte(Curve11 + y - 7);
                }
                break;
            case KBarOrientLR:
                tmp = pgm_read_byte(Curve18 + y);
                xw = width - 17 + tmp;
                if (y < 7) {
                    xl = left;
                } else {
                    xl = left + width - lbarwidth - 10 + pgm_read_byte(Curve11 + y - 7);
                    xw += left - xl;
                }
                break;
            case KBarOrientUL:
                tmp = pgm_read_byte(Curve18 + 17 - y);
                xl = left + 17 - tmp;
                if (y < 11) {
                    xw = left + lbarwidth + 10 - pgm_read_byte(Curve11 + 10 - y);
                } else {
                    xw = width - 17 + tmp;
                }
                break;
            case KBarOrientUR:
                tmp = pgm_read_byte(Curve18 + 17 - y);
                xw = width - 17 + tmp;
                if (y < 11) {
                    xl = left + width - lbarwidth - 10 + pgm_read_byte(Curve11 + 10 - y);
                    xw += left - xl;
                } else {
                    xl = left;
                }
                break;
        }

        writeFastHLine(xl,y+top,xw,textbgcolor);
    }
    endWrite();    
}

/************************************************************************/
/*                                                                      */
/*  Support Routines                                                    */
/*                                                                      */
/************************************************************************/

/*  AdaUI::charWidth
 *
 *      Character width
 */

int16_t AdaUI::charWidth(uint8_t ch)
{
    uint8_t first = pgm_read_byte(&gfxFont->first);
    GFXglyph *glyph = &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[ch - first]);
    return (uint8_t)pgm_read_byte(&glyph->xAdvance);
}

/*  AdaUI::stringWidth
 *
 *      String width. All we need to do is scan all of the characters
 *  and add up their widths.
 */

int16_t AdaUI::stringWidth(const char *str)
{
    int16_t w = 0;

    if (gfxFont) {
        uint8_t first = pgm_read_byte(&gfxFont->first);
        uint8_t last  = pgm_read_byte(&gfxFont->last);
        uint8_t ch;
        
        while (0 != (ch = (uint8_t)*str++)) {
            if ((first <= ch) && (ch <= last)) {
                w += charWidth(ch);
            }
        }
    } else {
        while (*str++) w += 6;
        w *= textsize;
    }
    return w;
}

/*  AdaUI::stringWidth
 *
 *      String width. All we need to do is scan all of the characters
 *  and add up their widths.
 */

int16_t AdaUI::stringWidth(const __FlashStringHelper *s)
{ 
    uint8_t *str = (uint8_t *)s;
    int16_t w = 0;

    if (gfxFont) {
        uint8_t first = pgm_read_byte(&gfxFont->first);
        uint8_t last  = pgm_read_byte(&gfxFont->last);
        uint8_t ch;
        
        while (0 != (ch = (uint8_t)pgm_read_byte(str++))) {
            if ((first <= ch) && (ch <= last)) {
                w += charWidth(ch);
            }
        }
    } else {
        while (pgm_read_byte(str++)) w += 6;
        w *= textsize;
    }
    
    return w;
}

/************************************************************************/
/*                                                                      */
/*  Blink free drawing                                                  */
/*                                                                      */
/************************************************************************/

#if ADAUI_NOBLINK == 1

/*
 *  This draws a band of the rounded rectangle from l to r. We assume l and r
 *  are within the range [x,x+w). Assumes startWrite already called. This is
 *  used for blink-free redrawing of buttons.
 */

void AdaUI::drawButton(AdaUIRect rect, AdaUICorner corners, int16_t l, int16_t r)
{
    int16_t xtmp;
    int16_t rpos;
    int16_t ypos;
    int16_t hlen;
    int16_t xpos;
    uint8_t tmp;
    
    /*
     *  Draw the left curve if visible
     */
     
    if (corners & (KCornerUL | KCornerLL)) {
        xtmp = rect.x + 11;
        /*
         *  0 to 10 has a curve. 
         */
        
        if (l < xtmp) {
            /*
             *  Draw individual vertical strokes to fill
             */
            
            rpos = r;
            if (rpos > xtmp) rpos = xtmp;
            
            for (xpos = l; xpos < rpos; ++xpos) {
                tmp = 10 - pgm_read_byte(Curve11 + (xpos - rect.x));
                
                ypos = rect.y;
                hlen = rect.h;
                if (corners & KCornerUL) {
                    ypos += tmp;
                    hlen -= tmp;
                }
                
                if (corners & KCornerLL) {
                    hlen -= tmp;
                }
                
                if (hlen > 0) writeFastVLine(xpos,ypos,hlen,textbgcolor);
            }
            
            l = xtmp;
        }
    }
    
    /*
     *  Draw the middle rectangle if visible
     */
    
    rpos = r;
    if (corners & (KCornerUR | KCornerLR)) {
        xtmp = rect.x + rect.w - 11;
        if (rpos > xtmp) rpos = xtmp;
    }
    
    if (l < rpos) {
        writeFillRect(l,rect.y,rpos-l,rect.h,textbgcolor);
    }
    
    /*
     *  Draw the right curve.
     */
        
    if (corners & (KCornerUR | KCornerLR)) {
        xtmp = rect.x + rect.w - 11;
        if (r > xtmp) {
            if (l < xtmp) l = xtmp;
            
            for (xpos = l; xpos < r; ++xpos) {
                tmp = 10 - pgm_read_byte(Curve11 + 10 - (xpos - xtmp));
                
                ypos = rect.y;
                hlen = rect.h;
                if (corners & KCornerUR) {
                    ypos += tmp;
                    hlen -= tmp;
                }
                
                if (corners & KCornerLR) {
                    hlen -= tmp;
                }
                
                if (hlen > 0) writeFastVLine(xpos,ypos,hlen,textbgcolor);
            }
            
            l = xtmp;
        }
    }
}


/*
 *  Internal drawing routine
 */

void AdaUI::drawButtonInternal(AdaUIRect r, void *s, bool inPgm, 
        int16_t baseline, AdaUICorner corners, AdaUIAlignment align)
{
    int16_t xstart;
    
    /*
     *  Find the position of the text to draw
     */
    
    int16_t sw;
    if (inPgm) {
        sw = stringWidth((const __FlashStringHelper *)s);
    } else {
        sw = stringWidth((const char *)s);
    }
    switch (align) {
        case KLeftAlign:
            xstart = r.x + 5;
            break;
        case KCenterAlign:
            xstart = r.x+(r.w-sw)/2;
            break;
        default:
        case KRightAlign:
            xstart = r.x+r.w-5-sw;
            break;
    }

    /*
     *  Draw the object in segments to prevent flashing
     */

    if (gfxFont == NULL) {
        int16_t xpos,ypos;
        int16_t width,height;

        /*
         *  Set cursor for drawing
         */
         
        setCursor(xstart,r.y+baseline-textsize * 6);
        
        /*
         *  The default (without a font) can draw the background. So we
         *  draw the text and then the box around the text
         */
        
        if (inPgm) {
            getTextBounds((const __FlashStringHelper *)s,cursor_x,cursor_y,&xpos,&ypos,&width,&height);
        } else {
            getTextBounds((const char *)s,cursor_x,cursor_y,&xpos,&ypos,&width,&height);
        }
        
        startWrite();
        drawButton(r,corners,r.x,xpos);
        drawButton(r,corners,xpos+width,r.x+r.w);
        drawButton(RECT(r.x,r.y,r.w,ypos-r.y),corners & (KCornerUL | KCornerUR),xpos,xpos+width);
        drawButton(RECT(r.x,ypos+height,r.w,r.y+r.h-ypos-height),corners & (KCornerLL | KCornerLR),xpos,xpos+width);
        endWrite();
                
        // This is a kludge from Adafruit GFX to handle the baseline of the old
        // style fonts. Origin of old fonts are the upper-left rather than
        // the baseline.
        if (inPgm) {
            print((const __FlashStringHelper *)s);
        } else {
            print((const char *)s);
        }
        
    } else {
        /*
         *  TODO: The textsize parameter grows the size of our font by
         *  the amount specified, but I don't want to multiply textsize
         *  everywhere. Figure out a way to refactor this so we don't
         *  write so damned much duplicate code.
         */
        
        int16_t xpos;
        uint8_t ch;
        uint8_t first = (uint8_t)pgm_read_byte(&gfxFont->first);
        uint8_t last = (uint8_t)pgm_read_byte(&gfxFont->last);
        uint8_t *bitmap = (uint8_t *)pgm_read_pointer(&gfxFont->bitmap);

        startWrite();

        /*
         *  This has some heavy lifting shit going on. Basically we draw our
         *  button in segments around each individual character. We reduce
         *  blinking by drawing each pixel only once when possible, but given 
         *  the way gfxFonts are put together, that requires a lot of effort.
         */
    
        /*
         *  Draw the left margin. If our font draws pixels that overlap left,
         *  we wind up blinking those pixels.
         */
        
        drawButton(r,corners,r.x,xstart);
        
        /*
         *  We use xpos to indicate the leftmost portion of our button that
         *  we have thus far drawn. This follows the boundaries of our
         *  character bitmaps rather than the boundaries of the font
         *  cursor position.
         *
         *  We use the cursor position variables to indicate the actual
         *  position where we're drawing the font.
         */
        
        xpos = xstart;
        cursor_x = xpos;
        cursor_y = r.y + baseline;
        
        /*
         *  Now work our way across, drawing the characters. We rewrite the 
         *  character drawing mechanism here, because we want to fill the
         *  background images.
         */
        
        const uint8_t *str = (const uint8_t *)s;
        for (;;) {
            if (inPgm) {
                ch = (uint8_t)pgm_read_byte(str++);
            } else {
                ch = *str++;
            }
            if (0 == ch) break;

            if ((first <= ch) && (ch <= last)) {
                /*
                 *  Get the glyph information
                 */
                
                GFXglyph *glyph = &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[ch-first]);
                uint16_t goffset = pgm_read_word(&glyph->bitmapOffset);
                uint8_t gw = pgm_read_byte(&glyph->width);
                uint8_t gh = pgm_read_byte(&glyph->height);
                int8_t  gx = pgm_read_byte(&glyph->xOffset);
                int8_t  gy = pgm_read_byte(&glyph->yOffset);
                
                /*
                 *  The bitmap we're drawing starts at cursor_x + gx.
                 *  If this is larger than xpos, this indicates a vertical
                 *  blank space that needs to be filled.
                 */
                
                int16_t tmpw = cursor_x + gx;
                if (tmpw > xpos) {
                    drawButton(r,corners,xpos,tmpw);
                    xpos = tmpw;
                }
                
                /*
                 *  Now we need to draw the top and bottom margins around our
                 *  bitmap. We do this in case we get the calcs wrong
                 */
                
                drawButton(RECT(r.x,r.y,r.w,cursor_y + gy - r.y),corners & (KCornerUL | KCornerUR),
                    cursor_x + gx,cursor_x + gx + gw);
                drawButton(RECT(r.x,cursor_y + gy + gh,r.w,r.y+r.h - cursor_y - gy - gh),
                    corners & (KCornerLL | KCornerLR),cursor_x + gx,cursor_x + gx + gw);
                
                /*
                 *  Draw the character bitmap itself. Our character is
                 *  drawn the same way as in the Adafruit GFX lib.
                 */
                
                uint8_t bit = 0;
                uint8_t bits;
                for (uint8_t yy = 0; yy < gh; ++yy) {
                    for (uint8_t xx = 0; xx < gw; ++xx) {
                        if (!(bit++ & 7)) {
                            bits = pgm_read_byte(bitmap + (goffset++));
                        }
                        
                        writePixel(xx+cursor_x+gx,yy+cursor_y+gy,(bits & 0x80) ? textcolor : textbgcolor);
                        
                        bits <<= 1;
                    }
                }
                
                /*
                 *  Now advance the cursor and the left margin
                 */
                
                // Move to the right of our bitmap
                xpos = cursor_x + gx + gw;
                
                // Now move the cursor
                cursor_x += (uint8_t)pgm_read_byte(&glyph->xAdvance);
            }
        }
        
        /*
         *  At this point we've drawn our button up to the xpos. Draw the
         *  rest
         */
        
        drawButton(r,corners,xpos,r.x+r.w);
        endWrite();
    }
}

#endif

/************************************************************************/
/*                                                                      */
/*  Round Rect Drawing                                                  */
/*                                                                      */
/************************************************************************/

/*  AdaUI::drawButton
 *
 *      Draw the button without text block.
 */

void AdaUI::drawButton(AdaUIRect r, AdaUICorner corners)
{
    int8_t tmp;
    int16_t xl,xw;

    /*
     *  Fast escape if this is just a rectangle
     */

    if (corners == 0) {
        fillRect(r.x,r.y,r.w,r.h,textbgcolor);
    } else {
        startWrite();
        /*
         *  We're drawing corners. Note this can look wonky if the size
         *  of our rectangle is too small.
         */
        
        if (corners & (KCornerUL | KCornerUR)) {
            /*
             *  Draw the top curves
             */
            
            for (uint8_t i = 0; i < 11; ++i) {
                tmp = 10 - pgm_read_byte(Curve11 + i);
                if (corners & KCornerUL) {
                    xl = r.x + tmp;
                    xw = r.w - tmp;
                } else {
                    xl = r.x;
                    xw = r.w;
                }
                if (corners & KCornerUR) {
                    xw -= tmp;
                }
                writeFastHLine(xl,i+r.y,xw,textbgcolor);
            }

            r.y += 11;
            r.h -= 11;
        }
        
        if (corners & (KCornerLL | KCornerLR)) {
            /*
             *  Draw the bottom curves
             */
            
            for (uint8_t i = 0; i < 11; ++i) {
                tmp = 10 - pgm_read_byte(Curve11 + 10 - i);
                if (corners & KCornerLL) {
                    xl = r.x + tmp;
                    xw = r.w - tmp;
                } else {
                    xl = r.x;
                    xw = r.w;
                }
                if (corners & KCornerLR) {
                    xw -= tmp;
                }
                writeFastHLine(xl,r.y+r.h-11+i,xw,textbgcolor);
            }
            
            r.h -= 11;
        }
        
        /*
         *  Draw the rectangle in between
         */
        
        if (r.h > 0) {
            writeFillRect(r.x,r.y,r.w,r.h,textbgcolor);
        }

        endWrite();
    }
}

void AdaUI::drawButton(AdaUIRect r, const char *str, int16_t baseline, 
        AdaUICorner corners, AdaUIAlignment align)
{
#if ADAUI_NOBLINK == 1
    drawButtonInternal(r,(const uint8_t *)str,false,baseline,corners,align);
#else
    int16_t xp;

    xp = stringWidth(str);
    switch (align) {
        case KLeftAlign:
            xp = r.x + 5;
            break;
        case KCenterAlign:
            xp = r.x+(r.w-xp)/2;
            break;
        default:
        case KRightAlign:
            xp = r.x+r.w-5-xp;
            break;
    }
    
    drawButton(r,corners);
    setCursor(xp,r.y+baseline);
    print(str);
#endif
}

void AdaUI::drawButton(AdaUIRect r, const __FlashStringHelper *str, 
        int16_t baseline, AdaUICorner corners, AdaUIAlignment align)
{
#if ADAUI_NOBLINK == 1
    drawButtonInternal(r,(const uint8_t *)str,true,baseline,corners,align);
#else
    int16_t xp;

    xp = stringWidth(str);
    switch (align) {
        case KLeftAlign:
            xp = r.x + 5;
            break;
        case KCenterAlign:
            xp = r.x+(r.w-xp)/2;
            break;
        default:
        case KRightAlign:
            xp = r.x+r.w-5-xp;
            break;
    }
    
    drawButton(r,corners);
    setCursor(xp,r.y+baseline);
    print(str);
#endif
}


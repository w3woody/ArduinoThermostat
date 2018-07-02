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

#include <SPI.h>       // this is needed for display
#include <Wire.h>      // this is needed for FT6206
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>
#include "AdaUI.h"
#include "Narrow25.h"
#include "Narrow75D.h"

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10
AdaUI context(TFT_CS, TFT_DC);

// The FT6206 uses hardware I2C (SCL/SDA)
Adafruit_FT6206 touch = Adafruit_FT6206();


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

void drawarc(int16_t x, int16_t y)
{
    int xc = x;
    int yc = y;
    const uint8_t *p = Arc_bitmap;
    uint8_t c;
    
    context.startWrite();
    
    while (0 != (c = pgm_read_byte(p++))) {
        if (c == 0x80) {
            yc++;
            xc = x;
        } else if (c & 0x80) {
            // Horizontal band
            context.writeFastHLine(xc,yc,0x7F & c,ADAUI_YELLOW);
        }
        xc += c & 0x7F;
    }
    
    context.endWrite();
}

void formatTemperature(char *dest, uint8_t temp)
{
    uint8_t pos = 0;
    uint8_t alt;
    
    if (temp == 0) {
        *dest++ = '0';
        *dest++ = 0;
    } else {
        while (temp > 0) {
            dest[pos++] = '0' + (temp % 10);
            temp /= 10;
        }
        alt = 0;
        dest[pos] = 0;
        --pos;
        while (alt < pos) {
            char c = dest[alt];
            dest[alt] = dest[pos];
            dest[pos] = c;
            ++alt;
            --pos;
        }
    }
}

void drawTemperatureMarker(uint8_t temp, uint16_t color)
{
    int16_t n = temp;
    if (n < 50) n = 50;
    if (n > 90) n = 90;
    double angle = ((3*M_PI/2) * (90 - n))/40 - (M_PI/4);

    double x = cos(angle);
    double y = - sin(angle);

    int16_t x1 = 200 + x * 75;
    int16_t y1 = 150 + y * 75;
    int16_t x2 = 200 + x * 85;
    int16_t y2 = 150 + y * 85;
    context.drawLine(x1,y1,x2,y2,color);       // white
    context.fillCircle(x2,y2,5,color);
    
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
    
    char buffer[8];
    formatTemperature(buffer,temp);
    context.drawButton(x2,y2-19,22,24,buffer,19,0,KCenterAlign);
}

void drawTemperature(uint8_t temp)
{
    int16_t n = temp;
    if (n < 50) n = 50;
    if (n > 90) n = 90;
    double angle = ((3*M_PI/2) * (90 - n))/40 - (M_PI/4);

    double x = cos(angle);
    double y = - sin(angle);

    int16_t x1 = 200 + x * 75;
    int16_t y1 = 150 + y * 75;
    int16_t x2 = 200 + x * 65;
    int16_t y2 = 150 + y * 65;      // inward
    context.drawLine(x1,y1,x2,y2,0xFFFF);       // white
    context.fillCircle(x2,y2,5,0xFFFF);
    
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
    
    // Draw the temperature
    char buffer[8];
    formatTemperature(buffer,temp);
    
    context.setTextColor(0xFFFF,ADAUI_BLACK);
    context.setFont(&Narrow75D);
    context.drawButton(160,120,80,40,buffer,30,0,KCenterAlign);
}

void setup() 
{
    /*
     *  Start screen
     */
    
    context.begin();
    context.setRotation(1);
    context.fillScreen(ADAUI_BLACK);
    
    /*
     *  Start touch
     */
    
    touch.begin(40);

    /*
     *  Draw a sample screen using our tools
     */
    
    context.setFont(&Narrow25);
//    context.setTextColor(ADAUI_BLUE,ADAUI_BLACK);
//    context.drawButton(0,0,80,32,F("\177DONE"),24,0,KLeftAlign);
    
    context.setTextColor(ADAUI_RED,ADAUI_BLACK);
    context.drawButton(240,0,80,32,F("TUE 12:52PM"),24);
    
    context.setTextColor(ADAUI_BLACK,ADAUI_RED);
    context.drawTopBar();
    
    // Draw side controls
    context.setTextColor(ADAUI_BLACK,ADAUI_BLUE);
    context.drawButton(0,51,80,37,F("FAN"),30);
    context.drawButton(0,89,80,37,F("PROGRAM"),30);

    context.setTextColor(ADAUI_BLACK,ADAUI_RED);
    context.drawButton(0,127,80,75);

    context.setTextColor(ADAUI_BLACK,ADAUI_BLUE);
    context.drawButton(0,203,80,37,F("SETTINGS"),30);
    
    // Draw temperature arc
    drawarc(120,70);
    
    // Draw temperature adjustment
    context.setTextColor(ADAUI_BLUE,ADAUI_DARKGRAY);
    context.drawButton(160,200,40,37,F("68"),28,KCornerUL | KCornerLL,KCenterAlign);
    context.drawButton(201,200,40,37,F("78"),28,KCornerUR | KCornerLR,KCenterAlign);
    
    context.setTextColor(ADAUI_PURPLE,ADAUI_BLACK);
    context.setFont(&Narrow25);
    context.drawButton(160,160,80,30,F("WINTER"),26,0,KCenterAlign);
    
    // Draw the temperature
    drawTemperatureMarker(70,0xF800);       // red
    drawTemperatureMarker(78,0x001F);       // blue
    drawTemperature(75);
}

void loop() 
{
  // put your main code here, to run repeatedly:
    if (!touch.touched()) return;
    
    TS_Point p = touch.getPoint();
    
}

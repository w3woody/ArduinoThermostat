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

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10
AdaUI context(TFT_CS, TFT_DC);

void setup() 
{
    context.begin();
    context.setRotation(1);
    context.fillScreen(ILI9341_BLACK);

    context.setCursor(5,20);
    context.setFont(&Narrow25);
    context.setTextColor(ILI9341_RED);
    context.println(F("Hello world!"));
    
    context.setTextColor(ILI9341_BLACK,ADAUI_RED);
    
    context.drawTopBar( 50, KBarOrientLL, 10, 300, 80);
    context.drawTopBar( 70, KBarOrientUL, 10, 300, 80);
    context.drawTopBar( 90, KBarOrientUR, 10, 300, 100);
    context.drawTopBar(110, KBarOrientLR, 10, 300, 100);
    
    context.setTextColor(ILI9341_BLACK,ADAUI_BLUE);
    context.drawButton(10,130,100,22);
    context.drawButton(10,160,100,22,KCornerUL);
    
    context.setTextColor(ILI9341_BLACK,ADAUI_DARKGRAY);
    context.drawButton(120,160,100,22,KCornerUL | KCornerUR);
    context.drawButton(230,160,70,22,KCornerLL | KCornerLR);
    
    context.setTextColor(ILI9341_BLACK,ADAUI_GREEN);
    context.drawButton(10,186,100,37,F("BTN!."),28,KCornerUL | KCornerUR | KCornerLL | KCornerLR);
}

void loop() 
{
  // put your main code here, to run repeatedly:

}

/*  AdaUIScreen.cpp
 *
 *      We store the state for our drawing as a global, but I want to make 
 *  it available across our application. So they're defined here.
 */

#import "AdaUIScreen.h"

/************************************************************************/
/*                                                                      */
/*  Globals                                                             */
/*                                                                      */
/************************************************************************/


// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10
AdaUI GC(TFT_CS, TFT_DC);

// The FT6206 uses hardware I2C (SCL/SDA)
Adafruit_FT6206 Touch;


/*  AInitScreen
 *
 *      Start up the screen and touch screen stuff
 */

void AInitScreen(void)
{
    GC.begin();

    GC.setRotation(1);
    GC.fillScreen(ADAUI_BLACK);
    
    Touch.begin(40);
}

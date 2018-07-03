/*  AdaUIScreen.h
 *
 *      We store the state for our drawing as a global, but I want to make 
 *  it available across our application. So they're defined here.
 */

#ifndef _AdaUIScreen_h
#define _AdaUIScreen_h

#import "AdaUI.h"
#include <Adafruit_FT6206.h>

/************************************************************************/
/*                                                                      */
/*  Globals                                                             */
/*                                                                      */
/************************************************************************/

extern AdaUI GC;                /* Global screen drawing context */
extern Adafruit_FT6206 Touch;   /* Touch screen driver */

extern void AInitScreen(void);  /* Initialize GC/Touch */

#endif


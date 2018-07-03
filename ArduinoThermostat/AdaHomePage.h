/*  AdaHomePage.h
 *
 *      Provides the support for drawing our home page; the thermostat
 *  and the measurements.
 */

#ifndef _AdaHomePage_h
#define _AdaHomePage_h

#include "AdaUIPage.h"

/************************************************************************/
/*                                                                      */
/*  Home page                                                           */
/*                                                                      */
/************************************************************************/

/*  AdaHomePage
 *
 *      Draws our thermostat and controls the basic settings.
 */

class AdaHomePage: public AdaUIPage
{
    public:
                        AdaHomePage();
                        
        virtual void    drawTitle();        // Override if custom title        
        virtual void    drawContents();     // Override for custom contents

        virtual void    handleEvent(uint8_t ix);
        virtual void    periodicEvents();
};



#endif

/*  AdaTempPage.h
 *
 *      Allows the users to set the temperature of our thermostat.
 */

#ifndef _AdaTempPage_h
#define _AdaTempPage_h

#include "AdaUIPage.h"

/************************************************************************/
/*                                                                      */
/*  Home page                                                           */
/*                                                                      */
/************************************************************************/

/*  AdaTempPage
 *
 *      Draws our thermostat and controls the basic settings.
 */

class AdaTempPage: public AdaUIPage
{
    public:
                        AdaTempPage();
                        
        virtual void    drawContents();     // Override for custom contents

        virtual void    handleEvent(uint8_t ix);
};


#endif

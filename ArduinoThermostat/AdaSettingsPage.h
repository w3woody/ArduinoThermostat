/*  AdaSettingsPage.h
 *
 *      Allows the users to set the temperature of our thermostat.
 */

#ifndef _AdaSettingsPage_h
#define _AdaSettingsPage_h

#include "AdaUIPage.h"

/************************************************************************/
/*                                                                      */
/*  Home page                                                           */
/*                                                                      */
/************************************************************************/

/*  AdaSettingsPage
 *
 *      Draws our thermostat and controls the basic settings.
 */

class AdaSettingsPage: public AdaUIPage
{
    public:
                        AdaSettingsPage();
                        
        virtual void    drawContents();     // Override for custom contents

        virtual void    handleEvent(uint8_t ix);
};


#endif

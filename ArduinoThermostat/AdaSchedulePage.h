/*  AdaSchedulePage.h
 *
 *      Allows the users to set the temperature of our thermostat.
 */

#ifndef _AdaSchedulePage_h
#define _AdaSchedulePage_h

#include "AdaUIPage.h"

/************************************************************************/
/*                                                                      */
/*  Home page                                                           */
/*                                                                      */
/************************************************************************/

/*  AdaSchedulePage
 *
 *      Draws our thermostat and controls the basic settings.
 */

class AdaSchedulePage: public AdaUIPage
{
    public:
                        AdaSchedulePage();
                        
        virtual void    drawContents();     // Override for custom contents
        virtual void    handleEvent(uint8_t ix);
    private:
};


#endif

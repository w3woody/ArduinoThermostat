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
                        
        virtual void    viewWillAppear();
        virtual void    drawContents();
        virtual void    drawTitle();        // Override if custom title        
        virtual void    handleEvent(uint8_t ix);
        virtual void    handleTap(TS_Point pt); // Allow other tap handling
    private:
        uint8_t         selDOW;
        uint8_t         selSchedule;
        uint8_t         selItem;
};


#endif

/*  AdaSchedulePickerPage.h
 *
 *      Allows the users to set the temperature of our thermostat.
 */

#ifndef _AdaSchedulePickerPage_h
#define _AdaSchedulePickerPage_h

#include "AdaUIPage.h"

/************************************************************************/
/*                                                                      */
/*  Home page                                                           */
/*                                                                      */
/************************************************************************/

/*  AdaSchedulePickerPage
 *
 *      Draws our thermostat and controls the basic settings.
 */

class AdaSchedulePickerPage: public AdaUIPage
{
    public:
                        AdaSchedulePickerPage();
                        
        virtual void    drawContents();
        virtual void    handleEvent(uint8_t ix);
        
    private:
        void            redrawSelected();
};


#endif

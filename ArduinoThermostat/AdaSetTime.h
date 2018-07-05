/*  AdaSetTime.h
 *
 *      Allows the users to set the temperature of our thermostat.
 */

#ifndef _AdaSetTime_h
#define _AdaSetTime_h

#include "AdaUIPage.h"

/************************************************************************/
/*                                                                      */
/*  Home page                                                           */
/*                                                                      */
/************************************************************************/

/*  AdaSetTime
 *
 *      Draws our thermostat and controls the basic settings.
 */

class AdaSetTimePage: public AdaUIPage
{
    public:
                        AdaSetTimePage();
                        
        virtual void    drawContents();     // Override for custom contents

        virtual void    handleEvent(uint8_t ix);
        virtual void    viewWillAppear();

    private:
        void            drawTime();
        void            setTime();
        
        uint8_t         hour;
        uint8_t         minute;
};

#endif

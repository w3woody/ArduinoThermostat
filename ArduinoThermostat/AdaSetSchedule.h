/*  AdaSetSchedule.h
 *
 *      Allows the users to set the temperature of our thermostat.
 */

#ifndef _AdaSetSchedule_h
#define _AdaSetSchedule_h

#include "AdaSetTime.h"

/************************************************************************/
/*                                                                      */
/*  Callback                                                            */
/*                                                                      */
/************************************************************************/

typedef 

/************************************************************************/
/*                                                                      */
/*  Home page                                                           */
/*                                                                      */
/************************************************************************/

/*  AdaSetSchedule
 *
 *      This extends our time page to deal with setting a schedule entry.
 */

class AdaSetSchedulePage: public AdaSetTimePage
{
    public:
                        AdaSetSchedulePage();
                        
        virtual void    drawContents();     // Override for custom contents

        uint8_t         heat;
        uint8_t         cool;

    private:
        void            drawHeat();
        void            drawCool();                
};

#endif

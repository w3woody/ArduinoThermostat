/*  AdaSetDate.h
 *
 *      Allows the users to set the temperature of our thermostat.
 */

#ifndef _AdaSetDate_h
#define _AdaSetDate_h

#include "AdaUIPage.h"
#include "AdaTime.h"

/************************************************************************/
/*                                                                      */
/*  Home page                                                           */
/*                                                                      */
/************************************************************************/

/*  AdaSetDatePage
 *
 *      Draws our thermostat and controls the basic settings.
 */

class AdaSetDatePage: public AdaUIPage
{
    public:
                        AdaSetDatePage();

        void            loadDate();
        virtual void    drawContents();     // Override for custom contents

        virtual void    handleEvent(uint8_t ix);
        virtual void    handleTap(TS_Point pt);
        
    private:
        void            drawMonth();
        void            drawYear();
        void            drawCalendar();
        
        void            setDate();
        
        uint8_t         day;
        uint8_t         month;
        uint16_t        year;
        
        uint8_t         mdow;
        uint8_t         mdays;
        uint8_t         mweeks;
        uint8_t         mheight;
};


#endif

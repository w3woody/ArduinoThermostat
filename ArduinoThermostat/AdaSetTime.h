/*  AdaSetTime.h
 *
 *      Allows the users to set the temperature of our thermostat.
 */

#ifndef _AdaSetTime_h
#define _AdaSetTime_h

#include "AdaUIPage.h"

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

/*  AdaSetTime
 *
 *      Draws our basic time setting widget. This is deliberately designed
 *  so it can be overridden.
 */

class AdaSetTimePage: public AdaUIPage
{
    public:
                        AdaSetTimePage();
                        
        virtual void    drawContents();     // Override for custom contents

        virtual void    handleTap(TS_Point pt); // Allow other tap handling
        virtual void    viewWillAppear();
        virtual void    viewWillDisappear();
                
        uint8_t         hour;               // pointer to result array [2 bytes]
        uint8_t         minute;             // pointer to result array [2 bytes]
        bool            changed;            // Indicate we changed.
        
    protected:
        // Alternate constructor
                        AdaSetTimePage(uint8_t offset, const AdaPage *p);

    private:
        uint8_t         offset;             // Offset of our display
        uint8_t         curPos;
        bool            isPM;
        char            digit[4];
        
        void            drawTime();
        void            drawKeyboard();
};

#endif

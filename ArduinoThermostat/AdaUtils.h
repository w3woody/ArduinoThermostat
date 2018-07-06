/*  AdaUtilsPage.h
 *
 *      Allows the users to set the temperature of our thermostat.
 */

#ifndef _AdaUtilsPage_h
#define _AdaUtilsPage_h

#include <stdint.h>
#include "AdaUIScreen.h"

/************************************************************************/
/*                                                                      */
/*  Utilities                                                           */
/*                                                                      */
/************************************************************************/

bool PtInRect(TS_Point pt, AdaUIRect r);

extern void FormatNumber(char *dest, uint16_t temp);
extern void FormatDayTime(char *dest, uint32_t time);
extern void FormatTime(char *ptr, uint8_t hour, uint8_t minute);
extern void FormatTime(char *dest, uint32_t time);
extern void FormatDate(char *dest, uint32_t time);

#endif

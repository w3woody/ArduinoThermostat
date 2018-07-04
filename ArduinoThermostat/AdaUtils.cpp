/*  AdaUtilsPage.cpp
 *
 *      Defines the behavior of our home page, which displays the thermostat.
 */

#include "AdaUtils.h"
#include "AdaTime.h"

#ifdef __AVR__
    #include <avr/pgmspace.h>
#elif defined(ESP8266) || defined(ESP32)
    #include <pgmspace.h>
#endif

#ifndef pgm_read_byte
    #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif

/************************************************************************/
/*                                                                      */
/*  Strings                                                             */
/*                                                                      */
/************************************************************************/

const char string_month[] PROGMEM = "JAN\0FEB\0MAR\0APR\0MAY\0JUN\0JUL\0AUG\0SEP\0OCT\0NOV\0DEC";
const char string_dow[]   PROGMEM = "SUN\0MON\0TUE\0WED\0THU\0FRI\0SAT";

/************************************************************************/
/*                                                                      */
/*  Utilities                                                           */
/*                                                                      */
/************************************************************************/

void FormatNumber(char *dest, uint16_t temp)
{
    uint8_t pos = 0;
    uint8_t alt;
    
    if (temp == 0) {
        *dest++ = '0';
        *dest++ = 0;
    } else {
        while (temp > 0) {
            dest[pos++] = '0' + (temp % 10);
            temp /= 10;
        }
        alt = 0;
        dest[pos] = 0;
        --pos;
        while (alt < pos) {
            char c = dest[alt];
            dest[alt] = dest[pos];
            dest[pos] = c;
            ++alt;
            --pos;
        }
    }
}

static void FormatTimeInternal(char *ptr, AdaTimeRecord &tr)
{
    bool pm;
    uint8_t hour = tr.hour;

    pm = hour > 12;
    hour %= 12;
    if (hour == 0) hour = 12;
    
    // Format hour/minute
    if (hour >= 10) {
        *ptr++ = '1';
        hour %= 10;
    }
    *ptr++ = '0' + hour;
    *ptr++ = ':';
    *ptr++ = '0' + (tr.min / 10);
    *ptr++ = '0' + (tr.min % 10);
    *ptr++ = ' ';
    if (pm) {
        *ptr++ = 'P';
    } else {
        *ptr++ = 'A';
    }
    *ptr++ = 'M';
    *ptr = 0;
}

/*
 *  Utility; append char from progmem
 */

static char *AppendChar(char *ptr, char *ref)
{
    char ch;
    
    while (0 != (ch = (char)pgm_read_byte(ref++))) *ptr++ = ch;
    *ptr++ = ' ';
    return ptr;
}

static char *FormatDay(char *ptr, uint8_t dow)
{
    return AppendChar(ptr,string_dow + dow * 4);
}

void FormatDayTime(char *ptr, uint32_t time)
{
    AdaTimeRecord tr = AdaSecToTime(time);
    ptr = FormatDay(ptr,tr.dow);
    FormatTimeInternal(ptr,tr);
}

void FormatTime(char *buffer, uint32_t time)
{
    AdaTimeRecord tr = AdaSecToTime(time);
    FormatTimeInternal(buffer,tr);
}

void FormatDate(char *ptr, uint32_t time)
{
    AdaTimeRecord tr = AdaSecToTime(time);
    
    ptr = FormatDay(ptr,tr.dow);
    
    ptr = AppendChar(ptr,string_month + (tr.month - 1) * 4);
    
    if (tr.day >= 10) {
        *ptr++ = '0' + tr.day/10;
    }
    *ptr++ = '0' + (tr.day % 10);
    *ptr++ = ',';
    *ptr++ = ' ';
    
    char buffer[8];
    FormatNumber(buffer,tr.year);
    for (char *c = buffer; *c; ++c) *ptr++ = *c;
    *ptr = 0;
}

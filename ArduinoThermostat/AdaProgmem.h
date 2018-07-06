/*  AdaProgmem.h
 *
 *      Common declarations for PROGMEM stuff.
 */

#ifndef _AdaProgmem_h
#define _AdaProgmem_h

#ifdef __AVR__
    #include <avr/pgmspace.h>
#elif defined(ESP8266) || defined(ESP32)
    #include <pgmspace.h>
#endif


/************************************************************************/
/*                                                                      */
/*  Constants                                                           */
/*                                                                      */
/************************************************************************/

/*
 *  Borrow code from Adafruit_GFX library to handle reading from PROGMEM
 *  space if we are missing some definitions
 */

#ifndef pgm_read_byte
    #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif

#ifndef pgm_read_word
    #define pgm_read_word(addr) (*(const unsigned short *)(addr))
#endif

#if !defined(__INT_MAX__) || (__INT_MAX__ > 0xFFFF)
    #define pgm_read_pointer(addr) ((void *)pgm_read_dword(addr))
#else
    #define pgm_read_pointer(addr) ((void *)pgm_read_word(addr))
#endif

#endif

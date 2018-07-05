//
//  main.cpp
//  BuildCurve
//
//  Created by William Woody on 7/1/18.
//  Copyright © 2018 Glenview Software. All rights reserved.
//

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
 *	This is a simple utility for generating a curve for our thermostat.
 *	We use a compression scheme that looks like:
 *
 *		$01-$7F: run from 1 to 127 pixels clear.
 *		$81-$FF: run from 1 to 127 pixels set
 *		$80: move to next line
 *		$00: done drawing.
 *
 *	This bitmap drawing method does well for big blocky things like our
 *	arc, but is not good for detail work
 */

#define BITMAPWIDTH		256			// width assumes divisible by 8
#define BITMAPHEIGHT	256

#define BYTEWIDTH		BITMAPWIDTH / 8
#define BUFFERSIZE		(BYTEWIDTH * BITMAPHEIGHT)

static int GChPos;
static uint8_t *GBuffer;

static void InitBuffer()
{
	GBuffer = (uint8_t *)malloc(BUFFERSIZE);
	memset(GBuffer,0,BUFFERSIZE);
}

static void setPixel(int16_t x, int16_t y)
{
	if ((x < 0) || (x >= BITMAPWIDTH) || (y < 0) || (y >= BITMAPHEIGHT)) return;

	int32_t pos = x + y * BITMAPWIDTH;
	int32_t byte = pos >> 3;
	uint8_t bit = ((uint8_t)0x80) >> (pos & 7);

	GBuffer[byte] |= bit;
}

static void clearPixel(int16_t x, int16_t y)
{
	if ((x < 0) || (x >= BITMAPWIDTH) || (y < 0) || (y >= BITMAPHEIGHT)) return;

	int32_t pos = x + y * BITMAPWIDTH;
	int32_t byte = pos >> 3;
	uint8_t bit = ((uint8_t)0x80) >> (pos & 7);

	GBuffer[byte] &= ~bit;
}



static bool testBit(int16_t x, int16_t y)
{
	if ((x < 0) || (x >= BITMAPWIDTH) || (y < 0) || (y >= BITMAPHEIGHT)) return false;

	int32_t pos = x + y * BITMAPWIDTH;
	int32_t byte = pos >> 3;
	uint8_t bit = ((uint8_t)0x80) >> (pos & 7);

	return (GBuffer[byte] & bit) ? true : false;
}

static void writeByte(uint8_t byte)
{
	if (GChPos > 7) {
		printf("\n");
		GChPos = 0;
	}
	printf("0x%02X,",byte);
	++GChPos;
}

static void dumpBytes()
{
	GChPos = 0;

	int lx = 0;		// Last pixel written
	int ly = 0;
	int xlen;

	for (int y = 0; y < BITMAPHEIGHT; ++y) {
		int x = 0;

		/*
		 *	Scan the line forward until we find a set bit
		 */

		for (;;) {
			while ((x < BITMAPWIDTH) && !testBit(x,y)) ++x;
			if (x >= BITMAPWIDTH) break;

			/*
			 *	At this point we need to write blank lines until we reach this
			 *	set bit
			 */

			while (ly < y) {
				writeByte(0x80);			// next line
				++ly;
				lx = 0;
			}
			while (0 < (xlen = (x - lx))) {
				if (xlen > 127) xlen = 127;
				writeByte(xlen);
				lx += xlen;
			}

			/*
			 *	Now scan forward as long as we find set bits
			 */

			while ((x < BITMAPWIDTH) && testBit(x,y)) ++x;
			while (0 < (xlen = (x - lx))) {
				if (xlen > 127) xlen = 127;
				writeByte(0x80 | xlen);
				lx += xlen;
			}
		}
	}

	writeByte(0);		// EOF
}


#include <iostream>

int main(int argc, const char * argv[])
{
	InitBuffer();

	/*
	 *	Draw our pattern. This is horribly inefficient, but it does the job.
	 */

//	for (int i = 10; i < 20; ++i) {
//		for (int j = 10; j < 20; ++j) {
//			setPixel(i,j);
//		}
//	}

//	for (double a = -M_PI/4; a < 5*M_PI/4; a += 0.01) {
//		double x = cos(a);
//		double y = -sin(a);
//
//		for (double r = 70; r < 80; r += 0.5) {
//			setPixel(floor(x*r + 0.5) + 80, floor(y*r + 0.5) + 80);
//		}
//	}

	for (double a = 0; a < M_PI*2; a += 0.01) {
		double x = cos(a);
		double y = -sin(a);
		for (double r = 0; r < 5; r += 0.5) {
			setPixel(floor(x*r + 0.5) + 5, floor(y*r + 0.5) + 5);
		}
	}

	dumpBytes();
	printf("\n");

	for (int n = 50; n <= 90; ++n) {
    	double angle = ((3*M_PI/2) * (90 - n))/40 - (M_PI/4);
		double x = cos(angle);
		double y = - sin(angle);

		int8_t xoff = x * 85;
		int8_t yoff = y * 85;

		printf("%d, %d,\n",xoff,yoff);
	}
}

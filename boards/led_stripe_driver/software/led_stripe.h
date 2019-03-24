
#ifndef LED_STRIPE_H
#define LED_STRIPE_H

//#include <unistd.h> // usleep
//#include <util/delay.h>

#include <stdint.h>

#include "light_ws2812_config.h"
#include "light_ws2812.h"


// 16MHz Clock
#define F_CPU 16000000UL
 
// Calculate the value needed for 
// the CTC match value in OCR1A.
#define CTC_MATCH_OVERFLOW ((F_CPU / 1000) / 8) 

// PF0 = Analog 0
// PC0 = Diginal 37
// src: https://www.arduino.cc/en/uploads/Hacking/PinMap2560big.png

#define arg_R0 arg0
#define arg_G0 arg1
#define arg_B0 arg2
#define arg_DT arg3
#define arg_MOD arg4

#define arg_R1 arg4
#define arg_G1 arg5
#define arg_B1 arg6

/**
 * MODE       ARG0  ARG1  ARG2  ARG3  ARG4  ARG5  ARG6
 *
 * FULL       Color
 * CHASER
 */

typedef enum {
	// LED setting
	FULL = 0, // all LEDs same color
	MOD, // light up every k'th LED in this color
	GRADIENT, // gradient (color0 -> color1)
	GRADIENT2, // double gradient (color0 -> color1 -> color0)

	// animation
	ROTATE,
	FADEOUT,
	FLASH
} Mode;

typedef struct {
	uint8_t mode;

	// Note: turning these into an array might cause issues with alignment
	uint8_t arg0;
	uint8_t arg1;
	uint8_t arg2;
	uint8_t arg3;
	uint8_t arg4;
	uint8_t arg5;
	uint8_t arg6;
} Command;

int main(void);
void setup(void);
void execute(Command*);
void clear_leds(void);

#endif


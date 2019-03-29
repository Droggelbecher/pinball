
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
#define CTC_MATCH_OVERFLOW ((F_CPU / 1000UL) / 8UL) 

// PF0 = Analog 0
// PC0 = Diginal 37
// src: https://www.arduino.cc/en/uploads/Hacking/PinMap2560big.png

typedef uint8_t Command[8];

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
	FLASH,

	_MAX = 0x0f
} Mode;

inline Mode mode(const Command c) { return c[0] & 0x0f; }
inline int  id  (const Command c) { return c[0] & 0xf0; }
inline int  r0  (const Command c) { return c[1]; }
inline int  g0  (const Command c) { return c[2]; }
inline int  b0  (const Command c) { return c[3]; }
inline int  dt  (const Command c) { return c[4]; }
inline int  mod (const Command c) { return c[5]; }
inline int  r1  (const Command c) { return c[5]; }
inline int  g1  (const Command c) { return c[6]; }
inline int  b1  (const Command c) { return c[7]; }

//typedef struct {
	//uint8_t mode;

	//// Note: turning these into an array might cause issues with alignment
	//uint8_t arg0;
	//uint8_t arg1;
	//uint8_t arg2;
	//uint8_t arg3;
	//uint8_t arg4;
	//uint8_t arg5;
	//uint8_t arg6;
//} Command;

int main(void);
void setup(void);
void execute(void);
void clear_leds(void);
void xfer_spi(void);
void swap_buffers(void);
uint8_t* active_buffer(void);
uint8_t* inactive_buffer(void);
void load(Command);

#endif

